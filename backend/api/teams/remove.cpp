#include "../../ascas.h"

auto TeamsRemove = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    int tid = stoi(argv[0]);
    TeamUtils.remove(tid);
    quickSendCode(200);
};