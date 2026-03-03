#pragma once

#include "utils.h"
#include "encrypt.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/types.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class SMTP {
    private:

    const int BUF_SIZE = 1024;

    int sock, conn;
    SSL* ssl;
    bool isSSL;
    std::string user;
    char* buf = new char[BUF_SIZE];

    int send(std::string msg) {
        int len = -1;
        if (!isSSL) len = ::send(conn, msg.c_str(), msg.size(), 0);
        else len = SSL_write(ssl, msg.c_str(), msg.size());
        MyAssert(len >= 0, "Failed to send data \"%s\" to server", msg.c_str());
        // std::cout << "To server: " << std::endl << msg;
        return len;
    }

    int recv(char* data, int len) {
        memset(data, 0, len);
        int res = -1;
        if (!isSSL) res = ::recv(conn, data, len, 0);
        else res = SSL_read(ssl, data, len);
        MyAssert(len >= 0, "Failed to receive data from server");
        // std::cerr << "From server: " << std::endl << data;
        return len;
    }

    public:

    SMTP(std::string host, int port, bool isSSL = false, bool isTLS = false) {
        isSSL |= isTLS; // TLS 时强制开启 SSL

        std::string ip = [&](){
            auto res = gethostbyname(host.c_str());
            return std::string(inet_ntoa(*(struct in_addr*)res->h_addr_list[0]));
        }();

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
        serverAddr.sin_port = htons(port);
        sock = socket(AF_INET, SOCK_STREAM, 0);
        MyAssert(sock >= 0, "Failed to create socket.");

        conn = connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        MyAssert(conn >= 0, "Failed to connect mail server.");
        conn = sock;

        if (isTLS) {
            send("STARTTLS\r\n");
            recv(buf, BUF_SIZE);
        }

        SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
        if (isSSL) {
            SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, conn);
            int res = SSL_connect(ssl);
            MyAssert(res != -1, "Failed to connect SSL");
            this->isSSL = isSSL;
        }
        recv(buf, BUF_SIZE);

        send("HELO MSG\r\n");
        recv(buf, BUF_SIZE);
    }

    void login(std::string user, std::string passwd) {
        this->user = user;

        send("AUTH LOGIN\r\n");
        recv(buf, BUF_SIZE);

        send(base64_encode(user.c_str(), user.size()) + "\r\n");
        recv(buf, BUF_SIZE);

        send(base64_encode(passwd.c_str(), passwd.size()) + "\r\n");
        recv(buf, BUF_SIZE);

        send("MAIL FROM:<" + user + ">\r\n");
        recv(buf, BUF_SIZE);
    }

    void rcpt(std::string user) {
        send("RCPT TO:<" + user + ">\r\n");
        recv(buf, BUF_SIZE);
    }

    void rcpt(std::vector<std::string> users) {
        for (auto v : users) rcpt(v);
    }

    void mail(std::string subject, std::string data) {
        send("DATA\r\n");
        recv(buf, BUF_SIZE);

        send("From: <" + user + ">\r\nSubject: " + subject + "\r\nMIME-Version: 1.0\r\nContent-Type: text/html\r\n\r\n" + data + "\r\n.\r\n");
        recv(buf, BUF_SIZE);

        send("QUIT\r\n");
        recv(buf, BUF_SIZE);

        close(conn);
    }
};