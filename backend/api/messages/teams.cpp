#include "../../ascas.h"

auto MessagesTeams = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    if (!TeamUtils.inTeam(uid, stoi(argv[0]))) quickSendCode(403);
    auto get = getParam(request);
    int maxmid = get.find("max") != get.end() ? stoi(get["max"]) : 2147483647;
    auto messages = MessageUtils.getTeamMessages(stoi(argv[0]), maxmid);
    quickSendItems(200, packarr(messages));
};