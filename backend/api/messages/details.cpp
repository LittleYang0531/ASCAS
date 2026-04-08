#include "../../ascas.h"

auto MessagesDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto get = getParam(request);
    int maxmid = get.find("max") != get.end() ? stoi(get["max"]) : INT_MAX;

    std::string talkId = argv[0];
    auto tmp = explode("-", talkId);
    if (tmp[0] == "users") {
        int uid1 = stoi(tmp[1]), uid2 = stoi(tmp[2]);
        if (uid != uid1 && uid != uid2) quickSendCode(403);
        auto messages = MessageUtils.getUsersMessages(uid, (uid != uid1 ? uid1 : uid2), maxmid);
        quickSendItems(200, packarr(messages));
    } else if (tmp[0] == "team") {
        int tid = stoi(tmp[1]);
        if (!TeamUtils.inTeam(uid, tid)) quickSendCode(403);
        auto messages = MessageUtils.getTeamMessages(tid, uid, maxmid);
        quickSendItems(200, packarr(messages));
    } else if (tmp[0] == "system") {
        if (system_channels.find(talkId) == system_channels.end()) quickSendCode(404);
        auto messages = MessageUtils.getSystemMessages(talkId, uid, maxmid);
        quickSendItems(200, packarr(messages));
    } else quickSendCode(404);
};