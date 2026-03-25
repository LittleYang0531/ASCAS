#include "../../ascas.h"

auto CropsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    if (!CropUtils.exists(cid)) quickSendCode(404);
    Crop item = CropUtils.getCropInfo(cid, uid);
    if (item.permission == UserPermission::NONE) quickSendCode(403);
    quickSendData(200, item);
};