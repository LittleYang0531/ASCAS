#include "../../ascas.h"

auto SensorsQuery = [](client_conn conn, http_request request, param argv) {
    int sid = stoi(argv[0]);
    if (!SensorUtils.exists(sid)) quickSendCode(404);
    Sensor sensor = SensorUtils.getSensorInfo(sid);
    auto get = getParam(request);
    time_t t = get.find("t") != get.end() ? stol(get["t"]) : time(NULL);
    int count = get.find("count") != get.end() ? stoi(get["count"]) : 100;
    auto res = SensorUtils.query(sid, t, count);
    auto items = packarr(res, [](std::pair<double, time_t> obj){ return jsonarr(obj.first, obj.second); });
    std::string unit = sensor.unit;
    quickSendData(200, items, unit);
};