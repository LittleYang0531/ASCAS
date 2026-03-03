#include "../ascas.h"

auto NotFound = [](client_conn conn, http_request request, param argv) {
    quickSendCode(404);
};