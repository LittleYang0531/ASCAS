#include "../../ascas.h"

auto UsersVerify = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    auto posts = json_decode(request.postdata);
    std::string code = posts["code"].asString();
    if (UserUtils.verify(code) == false) quickSendCode(404);
    quickSendCode(200);
};