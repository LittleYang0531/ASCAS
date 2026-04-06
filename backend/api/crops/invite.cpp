#include "../../ascas.h"

auto CropsInvite = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int cid = stoi(argv[0]);
    auto crop = CropUtils.getCropInfo(cid, uid);
    if (crop.permission != UserPermission::OWNER) quickSendCode(403);
    auto get = getParam(request);
    bool editor = get.find("editor") != get.end();
    auto uids = extarr<int>(json_decode(request.postdata));
    if (uids.size()) CropUtils.invite(cid, uid, uids, editor, request.argv["origin"]);
    quickSendCode(200);
};