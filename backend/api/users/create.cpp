#include "../../ascas.h"

auto UsersCreate = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    auto posts = postParam(request);
    std::string name = trim(posts["name"]);
    std::string passwd = trim(posts["passwd"]);
    std::string email = trim(posts["email"]);
    if (name == "" || passwd == "" || email == "") quickSendCode(400);
    if (UserUtils.checkName(name)) quickSendCode(409);
    if (UserUtils.checkEmail(email)) quickSendCode(409);
    UserUtils.create(User({
        .name = name,
        .email = email,
        .isAdmin = false,
    }), passwd, request.argv["origin"]);
    quickSendCode(200);
};