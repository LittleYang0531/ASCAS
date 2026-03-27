#include "../../ascas.h"
#include "../ping.cpp"

auto WSMessagesDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) {
        ws_exitRequest(conn);
        exit(0);
    }
    pthread_t pt;
    pthread_create(&pt, NULL, pingThread, (void*)&conn);

    Client client = Client("/tmp/ascas/msgDetails.sock");
    Connection c = client.connect();
    c.send("recv");
    c.send(std::to_string(uid));
    while (true) {
        std::string data = c.recv();
        auto exp = explode("\r\n", data);
        std::string talkId = exp[0];
        Message msg = json_decode(exp[1]).as<Message>();
        if (talkId != argv[0]) continue;
        MessageUtils.clearUnreadMark(talkId, uid);
        if (ws_send(conn, json_encode(Json::Value(msg))) < 0) break;
    }
    ws_exitRequest(conn);
    exit(0);
};