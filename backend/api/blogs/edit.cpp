#include "../../ascas.h"

auto BlogsEdit = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int bid = stoi(argv[0]);
    if (!BlogUtils.exists(bid)) quickSendCode(404);
    Blog blog = BlogUtils.getBlogInfo(bid);
    if (blog.author.uid != uid) quickSendCode(403);
    auto posts = json_decode(request.postdata);
    BlogUtils.edit(Blog({
        .bid = bid,
        .title = posts["title"].asString(),
        .content = posts["content"].asString(),
    }));
    quickSendCode(200);
};