#include "../../ascas.h"

auto UsersVerify = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    auto posts = postParam(request);
    std::string code = posts["code"];
    if (UserUtils.verify(code) == false) quickSendCode(404);
    quickSendCode(200);
};