#include "../../ascas.h"

auto BlogsCreate = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto posts = json_decode(request.postdata);
    int id = BlogUtils.create(uid, Blog({
        .title = posts["title"].asString(),
        .content = posts["content"].asString(),
    }));
    quickSendData(200, id);
};