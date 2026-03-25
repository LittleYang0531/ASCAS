#include "../../ascas.h"

auto MessagesSend = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto post = json_decode(request.postdata);
    int to = post["uid"].asInt();
    std::string message = post["message"].asString();
    MessageUtils.sendUserMessage(uid, to, message);
    quickSendCode(200);
};