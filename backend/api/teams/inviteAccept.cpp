#include "../../ascas.h"

auto TeamsInviteAccept = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int tid = stoi(argv[0]);
    auto get = getParam(request);
    std::string code = get["code"];
    if (!TeamUtils.checkInvite(tid, uid, code)) quickSendCode(404);
    TeamUtils.acceptInvite(tid, uid, code);
    quickSendCode(200);
};