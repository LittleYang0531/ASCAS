#include "../../ascas.h"

auto RecordsUploadImages = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    if (!CropUtils.exists(cid)) quickSendCode(404);
    Crop crop = CropUtils.getCropInfo(cid, uid);
    if (crop.permission != UserPermission::OWNER && crop.permission != UserPermission::EDITOR)
        quickSendCode(403);
    std::string image = base64_decode(request.postdata);
    if (image.size() > appConfig["images.maxSize"].asInt() * 1024) quickSendCode(413);
    std::string hash = sha1(image);
    std::ofstream fout("./data/images/" + hash + ".jpg", std::ios::binary);
    fout.write(image.c_str(), image.size());
    fout.close();
    quickSendData(200, hash);
};