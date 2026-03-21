
#include "../../ascas.h"

auto CropsRecordRemove = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    int rid = stoi(argv[1]);
    if(crop.permission != UserPermission::OWNER) quickSendCode(401);
    RecordUtils.remove(crop,rid);
    quickSendCode(200);
};
   