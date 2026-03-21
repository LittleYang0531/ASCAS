#include "../../ascas.h"

auto CropsRecordsAdd = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    int cid = stoi(argv[0]);
    if(crop.permission != UserPermission::OWNER 
        && crop.permission != UserPermission::EDITOR) quickSendCode(401);
    auto posts = json_decode(request.postdata); 
    RecordUtils.add(crop,posts,uid,cid);
    quickSendCode(200);
};
   

