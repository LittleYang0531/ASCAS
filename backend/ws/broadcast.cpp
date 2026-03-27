#pragma once
#include "../ascas.h"

std::function<void*(void*)> generateBroadcastServer(std::string sockPath) {
    return [sockPath](void* args) -> void* {
        proc_settitle(("ascas-backend: websocket central server " + sockPath).c_str());
        Server server = Server(sockPath);

        std::map<int, std::vector<Connection> > conns;
        while (true) {
            Connection conn = server.accept();
            std::string type = conn.recv();
            if (type == errorKey) continue;
            // std::cout << sockPath << ": " << type << ", id = " << conn.conn << std::endl;
            if (type == "recv") { // 信息接收者
                int uid = stoi(conn.recv());
                conns[uid].push_back(conn);
            } else { // 信息发送者
                int uid = stoi(conn.recv());
                std::string msg = conn.recv();
                int cnt = 0;
                if (conns.find(uid) != conns.end()) {
                    for (auto it = conns[uid].begin(); it != conns[uid].end(); ) {
                        if (!(*it).send(msg)) {
                            // std::cout << sockPath << ": " << type << ", id = " << conn.conn << ", erase id = " << (*it).conn << std::endl;
                            (*it).close();
                            it = conns[uid].erase(it);
                        } else {
                            // std::cout << sockPath << ": " << type << ", id = " << conn.conn << ", sent to id = " << (*it).conn << std::endl;
                            cnt++;
                            it++;
                        }
                    }
                    if (conns[uid].size() == 0) conns.erase(conns.find(uid));
                }
                conn.send(std::to_string(cnt));
                conn.close();
            }
        }
        return NULL;
    };
}