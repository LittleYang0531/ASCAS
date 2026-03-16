#include "../../ascas.h"

auto CropsRecordsList = [](client_conn conn, http_request request, param argv) {
    if(request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    auto posts = json_decode(request.postdata);
    int ret = RecordUtils.list(crop,posts);
    if(ret == -1) quickSendCode(404);
    quickSendCode(200);
};