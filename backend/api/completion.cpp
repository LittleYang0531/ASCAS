#include "../ascas.h"

auto Completion = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);

    std::string url = appConfig["ai.endPoint"].asString();
    std::string scheme = url.substr(0, url.find("://"));
    std::string host = url.substr(url.find("//") + 2);
    std::string path = host.substr(host.find("/"));
    host = host.substr(0, host.find("/"));
    int port = scheme == "https" ? 443 : 80;
    bool isSSL = scheme == "https";
    if (host.find(":") != std::string::npos) {
        port = stoi(host.substr(host.find(":") + 1));
        host = host.substr(0, host.find(":"));
    }
    std::string ip = [&](){
        auto res = gethostbyname(host.c_str());
        return std::string(inet_ntoa(*(struct in_addr*)res->h_addr_list[0]));
    }();

    Client client(ip, port);
    Connection conn2 = client.connect();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    SSL* ssl;
    if (isSSL) {
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, conn2.conn);
        if (SSL_connect(ssl) == -1) quickSendCode(500);
    }

    std::stringstream ss;
    std::string data = json_encode(jsonobj(
        "model", appConfig["ai.model"].asString(),
        "stream", true,
        "messages", json_decode(request.postdata)["messages"]
    ));
    ss << "POST " << path << "/chat/completions HTTP/1.1\r\n";
    ss << "Host: " << host << "\r\n";
    ss << "User-Agent: curl/8.5.0\r\n";
    ss << "Accept: */*\r\n";
    ss << "Authorization: Bearer " << appConfig["ai.apiKey"].asString() << "\r\n";
    ss << "Content-Type: application/json\r\n";
    ss << "Content-Length: " << data.size() << "\r\n\r\n";
    ss << data;
    if (!isSSL) send(conn2.conn, ss.str().c_str(), ss.str().size(), 0);
    else SSL_write(ssl, ss.str().c_str(), ss.str().size());

    bool first = true;
    std::string responseHeader = 
    "\r\n"
    "Access-Control-Allow-Credentials: true\r\n"
    "Access-Control-Allow-Headers: *\r\n"
    "Access-Control-Allow-Methods: *\r\n"
    "Access-Control-Allow-Origin: " + request.argv["origin"];
    while (true) {
        int len;
        char* ch = new char[1024];
        if (!isSSL) len = recv(conn2.conn, ch, 1024, 0);
        else len = SSL_read(ssl, ch, 1024);
        if (len < 0) exitRequest(conn);
        std::string data = std::string(ch, len);
        delete[] ch;
        if (first && data.find("\r\n\r\n") != std::string::npos) {
            data.replace(data.find("\r\n"), 0, responseHeader);
            first = false;
        }
        int siz = send(conn, data);
        if (siz == -1) exitRequest(conn);
        if (data.find("data: [DONE]") != std::string::npos) exitRequest(conn);
    }
};