#pragma once
#include "../ascas.h"

void* pingThread(void* args) {
    client_conn* conn = (client_conn*)args;
    while (true) {
        std::string data = ws_recv(*conn);
        if (data == ws_recv_error || data == "bye") {
            ws_exitRequest(*conn);
            exit(0);
        }
    }
    return NULL;
}