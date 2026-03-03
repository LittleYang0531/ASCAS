#include "../ascas.h"

auto Favicon = [](client_conn conn, http_request request, param argv) {
    StaticFileDownloader(conn, request, favicon_ico, "image/vnd.microsoft.icon");
};