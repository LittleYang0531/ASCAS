#include "../../ascas.h"

auto CropsRemove = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    if(crop.permission != UserPermission::OWNER) quickSendCode(401); 
    CropUtils.remove(crop);
    quickSendCode(200);
};