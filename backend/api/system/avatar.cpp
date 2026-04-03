#include "../../ascas.h"

auto SystemAvatar = [](client_conn conn, http_request request, param argv) {
    StaticFileDownloader(conn, request, "./data/system-avatars/" + argv[0] + ".png");
};