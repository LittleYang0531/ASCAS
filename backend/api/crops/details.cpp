#include "../../ascas.h"

auto CropsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    Crop item = CropUtils.getCropInfo(cid, uid);
    if (item.permission == UserPermission::NONE) quickSendCode(401);
    quickSendData(200, item);
};