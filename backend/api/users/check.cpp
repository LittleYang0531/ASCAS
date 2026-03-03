#include "../../ascas.h"

auto UsersCheck = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (uid == 0) quickSendCode(401);
    else quickSendItem(200, UserUtils.getUserInfo(uid).toJsonObject());
};