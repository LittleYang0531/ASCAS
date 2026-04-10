#include "../../ascas.h"

auto SensorsCreate = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto posts = json_decode(request.postdata);
    std::string title = posts["title"].asString();
    std::string unit = posts["unit"].asString();
    auto res = SensorUtils.create(Sensor({
        .owner = User({ .uid = uid }),
        .title = title,
        .unit = unit,
    }));
    int id = res.first;
    std::string sensorId = res.second;
    quickSendData(200, id, sensorId);
};