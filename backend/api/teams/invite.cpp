#include "../../ascas.h"

auto TeamsInvite = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int tid = stoi(argv[0]);
    auto team = TeamUtils.getTeamsInfo(tid);
    if (team.owner.uid != uid) quickSendCode(403);
    auto uids = extarr<int>(json_decode(request.postdata));
    if (uids.size()) TeamUtils.invite(tid, uids, request.argv["origin"]);
    quickSendCode(200);
};