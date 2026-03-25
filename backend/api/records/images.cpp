#include "../../ascas.h"

auto RecordsImages = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    if (!CropUtils.exists(cid)) quickSendCode(404);
    Crop crop = CropUtils.getCropInfo(cid, uid);
    if (crop.permission == UserPermission::NONE) quickSendCode(403);
    std::string path = "./data/images/" + argv[1] + ".jpg";
    if (!fileExists(path)) quickSendCode(404);
    StaticFileDownloader(conn, request, path);
};