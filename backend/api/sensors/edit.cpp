#include "../../ascas.h"

auto SensorsEdit = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int sid = stoi(argv[0]);
    if (!SensorUtils.exists(sid)) quickSendCode(404);
    int ownerid = SensorUtils.getSensorInfo(sid).owner.uid;
    if (ownerid != uid) quickSendCode(403);
    auto posts = json_decode(request.postdata);
    std::string title = posts["title"].asString();
    std::string unit = posts["unit"].asString();
    SensorUtils.edit(Sensor({
        .sid = sid,
        .title = title,
        .unit = unit
    }));
    quickSendCode(200);
};