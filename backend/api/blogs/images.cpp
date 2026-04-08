#include "../../ascas.h"

auto BlogsImages = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    std::string path = "./data/images/" + argv[1] + ".jpg";
    if (!fileExists(path)) quickSendCode(404);
    StaticFileDownloader(conn, request, path);
};