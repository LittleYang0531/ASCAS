#include "../../ascas.h"

auto CropsCreate = [](client_conn conn, http_request request, param argv) {
    
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto posts = json_decode(request.postdata);
    std::string title = posts["title"].asString();
    std::string description = posts["description"].asString();
    auto properties = extarr<RecordProperty>(posts["properties"]);
    User owner = UserUtils.getUserInfo(uid);
    auto editors = extarr<User>(posts["editors"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    auto viewers = extarr<User>(posts["viewers"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    if (title == "" || properties.size() == 0) quickSendCode(400);
    int id = CropUtils.create(Crop({
        .title = title,
        .description = description,
        .properties = properties,
        .owner = owner,
        .editors = editors,
        .viewers = viewers,
    }));
    quickSendData(200, id);
};