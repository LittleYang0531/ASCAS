#include "../../ascas.h"

auto UsersReset = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    auto posts = postParam(request);
    std::string type = posts["type"];
    std::string email = posts["email"];
    std::string code = posts["code"];
    std::string passwd = posts["passwd"];
    if (type == "request") {
        if (email == "") quickSendCode(400);
        if (UserUtils.reset_request(email, request.argv["origin"]) == false) quickSendCode(404);
        quickSendCode(200);
    } else if (type == "verify") {
        if (code == "") quickSendCode(400);
        if (UserUtils.reset_verify(code) == false) quickSendCode(404);
        quickSendCode(200);
    } else if (type == "reset") {
        if (code == "" || passwd == "") quickSendCode(400);
        if (UserUtils.reset(code, passwd) == false) quickSendCode(404);
        quickSendCode(200);
    } else quickSendCode(400);
};