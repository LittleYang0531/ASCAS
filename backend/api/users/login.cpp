#include "../../ascas.h"

auto UsersLogin = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    auto posts = json_decode(request.postdata);
    std::string name = trim(posts["name"].asString());
    std::string passwd = trim(posts["passwd"].asString());
    if (name == "" || passwd == "") quickSendCode(400);
    std::string session = UserUtils.login(name, passwd);
    int expires = appConfig["users.sessionExpireTime"].asInt() * 24 * 60 * 60;
    if (session == "") quickSendCode(401);
    if (session == "not_verified") quickSendCode(403);
    quickSendData(200, session, expires);
};