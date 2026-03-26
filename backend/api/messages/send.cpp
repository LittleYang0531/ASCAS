#include "../../ascas.h"

auto MessagesSend = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto userInfo = UserUtils.getUserInfo(uid);
    auto post = json_decode(request.postdata);
    std::string talkId = post["id"].asString();
    std::string message = post["message"].asString();

    auto tmp = explode("-", talkId);
    if (tmp[0] == "users") {
        int uid1 = stoi(tmp[1]), uid2 = stoi(tmp[2]);
        if (uid != uid1 && uid != uid2) quickSendCode(403);
        auto item = MessageUtils.sendUserMessage(uid, (uid != uid1 ? uid1 : uid2), message);
        item.user = userInfo;
        quickSendItem(200, item);
    } else if (tmp[0] == "team") {
        int tid = stoi(tmp[1]);
        if (!TeamUtils.inTeam(uid, tid)) quickSendCode(403);
        auto item = MessageUtils.sendTeamMessage(uid, tid, message);
        item.user = userInfo;
        quickSendItem(200, item);
    } else quickSendCode(404);
};