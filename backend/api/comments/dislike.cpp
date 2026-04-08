#include "../../ascas.h"

auto CommentsDislike = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[1]);
    if (!CommentUtils.exists(cid)) quickSendCode(404);
    User author = CommentUtils.getAuthor(cid);
    if (author.uid == uid) quickSendCode(403);
    CommentUtils.dislike(uid, cid);
    quickSendCode(200);
};