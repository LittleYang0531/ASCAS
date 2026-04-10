#include "../../ascas.h"

auto SensorsGenerate = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int sid = stoi(argv[0]);
    if (!SensorUtils.exists(sid)) quickSendCode(404);
    int ownerid = SensorUtils.getSensorInfo(sid).owner.uid;
    if (ownerid != uid) quickSendCode(403);
    std::string sensorId = SensorUtils.regenerate(sid);
    quickSendData(200, sensorId);
};