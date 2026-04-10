#include "../../ascas.h"

auto CropsRecordsEdit = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    int rid = stoi(argv[1]),cid = stoi(argv[0]);
    if(crop.permission != UserPermission::OWNER) quickSendCode(403);
    auto posts = json_decode(request.postdata); 
    RecordUtils.edit(crop,posts,rid,cid);
    quickSendCode(200);
};
   

