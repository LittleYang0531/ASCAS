#include "../../ascas.h"

auto CropsInviteAccept = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    auto get = getParam(request);
    std::string code = get["code"];
    if (!CropUtils.checkInvite(cid, uid, code)) quickSendCode(404);
    CropUtils.acceptInvite(cid, uid, code);
    quickSendCode(200);
};