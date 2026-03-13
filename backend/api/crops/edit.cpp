#include "../../ascas.h"

auto CropsEdit = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (uid == 0) quickSendCode(401);
    Crop previous_crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    if(previous_crop.permission != UserPermission::OWNER) quickSendCode(401);
    auto posts = json_decode(request.postdata); 
    std::string title = posts["title"].asString();
    std::string description = posts["description"].asString();
    User owner = UserUtils.getUserInfo(uid);
    auto editors = extarr<User>(posts["editors"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    auto viewers = extarr<User>(posts["viewers"], [](Json::Value obj){ return User({ .uid = obj.asInt() }); });
    auto properties = extarr<RecordProperty>(posts["properties"]);
    Crop current_crop = Crop({
        .cid = previous_crop.cid,
        .title = title,
        .description = description,
        .properties = properties,
        .owner = owner,
        .editors = editors,
        .viewers = viewers,
    });
    CropUtils.edit(previous_crop,current_crop);
    quickSendCode(200);
};


