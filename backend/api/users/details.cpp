#include "../../ascas.h"

auto UsersDetails = [](client_conn conn, http_request request, param argv) {
    int uid = stoi(argv[0]);
    if (!UserUtils.exists(uid)) quickSendCode(404);
    auto user = UserUtils.getUserInfo(uid);
    quickSendItem(200, user);
};