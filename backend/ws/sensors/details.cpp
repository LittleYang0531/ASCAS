#include "../../ascas.h"
#include "../ping.cpp"

auto WSSensorsDetails = [](client_conn conn, http_request request, param argv) {
    pthread_t pt;
    pthread_create(&pt, NULL, pingThread, (void*)&conn);
    int sid = stoi(argv[0]);

    Client client = Client("/tmp/ascas/sensorDetails.sock");
    Connection c = client.connect();
    c.send("recv");
    c.send(std::to_string(sid));
    while (true) {
        std::string msg = c.recv();
        if (ws_send(conn, msg) < 0) break;
    }
    ws_exitRequest(conn);
    exit(0);
};