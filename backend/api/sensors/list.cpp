#include "../../ascas.h"

auto SensorsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto get = getParam(request);
    auto sensors = SensorUtils.list(uid, get.find("ignore") != get.end());
    quickSendItems(200, packarr(sensors));
};