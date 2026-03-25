#include "../../ascas.h"

auto TeamsEdit = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (uid == 0) quickSendCode(401);            
    int tid = stoi(argv[0]);
    auto posts = json_decode(request.postdata); 
    std::string title = posts["title"].asString();
    std::string description = posts["description"].asString();
    auto members = extarr<User>(posts["members"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    User owner = UserUtils.getUserInfo(uid);
    Team oldteam = TeamUtils.getTeamsInfo(tid);
    if(oldteam.owner.uid != uid) quickSendCode(403);  
    Team newteam = Team({
        .tid = tid,
        .title = title,
        .description = description,
        .owner = owner,
        .members = members,
    });
    TeamUtils.edit(newteam);
    quickSendCode(200);
};


