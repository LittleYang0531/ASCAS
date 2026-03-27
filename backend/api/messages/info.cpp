#include "../../ascas.h"

auto MessagesInfo = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);

    std::string talkId = argv[0];
    auto tmp = explode("-", talkId);
    if (tmp[0] == "users") {
        int uid1 = stoi(tmp[1]), uid2 = stoi(tmp[2]);
        if (uid != uid1 && uid != uid2) quickSendCode(403);
        auto item = MessageUtils.getUsersTalkInfo(uid, uid != uid1 ? uid1 : uid2);
        quickSendItem(200, item);
    } else if (tmp[0] == "team") {
        int tid = stoi(tmp[1]);
        if (!TeamUtils.inTeam(uid, tid)) quickSendCode(403);
        auto item = MessageUtils.getTeamTalkInfo(tid);
        quickSendItem(200, item);
    } else quickSendCode(404);
};