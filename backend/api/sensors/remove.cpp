#include "../../ascas.h"

auto SensorsRemove = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int sid = stoi(argv[0]);
    if (!SensorUtils.exists(sid)) quickSendCode(404);
    int ownerid = SensorUtils.getSensorInfo(sid).owner.uid;
    if (ownerid != uid) quickSendCode(403);
    SensorUtils.remove(sid);
    quickSendCode(200);
};