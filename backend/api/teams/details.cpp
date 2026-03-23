#include "../../ascas.h"

auto TeamsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int tid = stoi(argv[0]);
    if (!TeamUtils.exists(tid)) quickSendCode(404);
    Team item = TeamUtils.getTeamsInfo(tid,uid);
    quickSendData(200, item);
};