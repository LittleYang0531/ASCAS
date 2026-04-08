#include "../../ascas.h"

auto UsersAvatar = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int req = stoi(argv[0]);
    if (request.method == "POST") {
        if (uid != req) quickSendCode(403);
        std::string data = base64_decode(request.postdata);
        if (data.size() > appConfig["avatars.maxSize"].asInt() * 1024) quickSendCode(413);
        std::ofstream fout("./data/avatars/" + std::to_string(req) + ".png", std::ios::binary);
        fout.write(data.c_str(), data.size());
        quickSendCode(200);
    }
    if (!UserUtils.exists(req)) quickSendCode(404);
    std::string avatarPath = "./data/avatars/" + std::to_string(req) + ".png";
    if (!fileExists(avatarPath)) writeImage(avatarPath, UserUtils.generateAvatar(std::to_string(req)));
    StaticFileDownloader(conn, request, avatarPath);
};