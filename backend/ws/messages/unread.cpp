#include "../../ascas.h"
#include "../ping.cpp"

auto WSMessagesUnread = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) {
        ws_send(conn, "0");
        ws_exitRequest(conn);
        exit(0);
    }
    pthread_t pt;
    pthread_create(&pt, NULL, pingThread, (void*)&conn);

    int unread = MessageUtils.getUnread(uid);
    ws_send(conn, std::to_string(unread));

    Client client = Client("/tmp/ascas/msgUnread.sock");
    Connection c = client.connect();
    c.send("recv");
    c.send(std::to_string(uid));
    while (true) {
        std::string msg = c.recv();
        if (ws_send(conn, msg) < 0) break;
    }
    ws_exitRequest(conn);
    exit(0);
};