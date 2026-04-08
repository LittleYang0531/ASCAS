#include "../../ascas.h"

auto CommentsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int bid = stoi(argv[0]);
    if (!BlogUtils.exists(bid)) quickSendCode(404);
    auto gets = getParam(request);
    int maxcid = gets["max"] == "" ? INT_MAX : stoi(gets["max"]);
    auto items = CommentUtils.list(uid, bid, maxcid);
    quickSendItems(200, packarr(items));
};