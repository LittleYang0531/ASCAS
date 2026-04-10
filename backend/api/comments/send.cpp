#include "../../ascas.h"

auto CommentsSend = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int bid = stoi(argv[0]);
    if (!BlogUtils.exists(bid)) quickSendCode(404);
    auto posts = json_decode(request.postdata);
    std::string msg = posts["msg"].asString();
    int id = CommentUtils.send(bid, uid, msg);
    quickSendData(200, id);
};