#include "../../ascas.h"

auto UsersGenerateAvatar = [](client_conn conn, http_request request, param argv) {
    std::string content = getParam(request)["s"];
    std::string avatar = writeImageToString(UserUtils.generateAvatar(content));
    argvar response = __api_default_response;
    response["Content-Type"] = "image/png";
    response["Content-Length"] = std::to_string(avatar.size());
    putRequest(conn, 200, response);
    send(conn, avatar);
    exitRequest(conn);
};