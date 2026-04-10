#include "../../ascas.h"

auto SensorsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto sensors = SensorUtils.list(uid);
    quickSendItems(200, packarr(sensors));
};