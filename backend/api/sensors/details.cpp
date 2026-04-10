#include "../../ascas.h"

auto SensorsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int sid = stoi(argv[0]);
    if (!SensorUtils.exists(sid)) quickSendCode(404);
    int ownerid = SensorUtils.getSensorInfo(sid).owner.uid;
    if (ownerid != uid) quickSendCode(403);
    Sensor sensor = SensorUtils.getSensorInfo(sid);
    quickSendItem(200, sensor);
};