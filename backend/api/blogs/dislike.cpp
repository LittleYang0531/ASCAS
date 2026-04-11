#include "../../ascas.h"

auto BlogsDislike = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int bid = stoi(argv[0]);
    if (!BlogUtils.exists(bid)) quickSendCode(404);
    // Blog blog = BlogUtils.getBlogInfo(bid);
    // if (blog.author.uid == uid) quickSendCode(403);
    BlogUtils.dislike(uid, bid);
    quickSendCode(200);
};