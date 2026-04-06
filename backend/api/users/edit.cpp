#include "../../ascas.h"

auto UsersEdit = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto posts = json_decode(request.postdata);
    std::string name = trim(posts["name"].asString());
    std::string email = trim(posts["email"].asString());
    std::string description = trim(posts["description"].asString());
    if (name == "" || email == "") quickSendCode(400);
    if (UserUtils.checkName(name, uid)) quickSendCode(409);
    if (UserUtils.checkEmail(email, uid)) quickSendCode(409);
    UserUtils.edit(User({
        .uid = uid,
        .name = name,
        .email = email,
        .isAdmin = false,
    }), description);
    quickSendCode(200);
};