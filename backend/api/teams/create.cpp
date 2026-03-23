#include "../../ascas.h"

auto TeamsCreate = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto posts = json_decode(request.postdata);
    std::string title = posts["title"].asString();
    std::string description = posts["description"].asString();
    User owner = UserUtils.getUserInfo(uid);
    std::vector<User> members = extarr<User>(posts["members"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    if (title == "") quickSendCode(400);
    int id = TeamUtils.create(Team({
        .title = title,
        .description = description,
        .owner = owner,
        .members = members,
    }));
    quickSendData(200, id);
};