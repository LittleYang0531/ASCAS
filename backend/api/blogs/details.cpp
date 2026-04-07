#include "../../ascas.h"

auto BlogsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int bid = stoi(argv[0]);
    if (!BlogUtils.exists(bid)) quickSendCode(404);
    auto item = BlogUtils.getBlogInfo(bid);
    int like = BlogUtils.checkLike(uid, bid);
    int dislike = BlogUtils.checkDislike(uid, bid);
    int star = BlogUtils.checkStar(uid, bid);
    quickSendData(200, item, like, dislike, star);
};