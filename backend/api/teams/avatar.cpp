#include "../../ascas.h"

auto TeamsAvatar = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int tid = stoi(argv[0]);
    if (!TeamUtils.exists(tid)) quickSendCode(404);
    if (request.method == "POST") {
        auto team = TeamUtils.getTeamsInfo(tid);
        if (team.owner.uid != uid) quickSendCode(403);
        std::string data = base64_decode(request.postdata);
        std::ofstream fout("./data/team-avatars/" + std::to_string(tid) + ".png", std::ios::binary);
        fout.write(data.c_str(), data.size());
        quickSendCode(200);
    }
    std::string avatarPath = "./data/team-avatars/" + std::to_string(tid) + ".png";
    if (!fileExists(avatarPath)) writeImage(avatarPath, UserUtils.generateAvatar("team-" + std::to_string(tid)));
    StaticFileDownloader(conn, request, avatarPath);
};