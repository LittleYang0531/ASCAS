#include "../../ascas.h"

auto SensorsAdd = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    std::string sensorId = argv[0];
    int sid = SensorUtils.checkSensorId(sensorId);
    if (!sid) quickSendCode(401);
    double data = stod(request.postdata);
    SensorUtils.pushValue(sid, data);
    quickSendCode(200);
};