#include "../../ascas.h"
#include "../ping.cpp"

auto WSSensorsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) {
        ws_exitRequest(conn);
        exit(0);
    }
    pthread_t pt;
    pthread_create(&pt, NULL, pingThread, (void*)&conn);

    Client client = Client("/tmp/ascas/sensorList.sock");
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