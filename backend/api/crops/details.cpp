#include "../../ascas.h"

auto CropsDetails = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    int cid = stoi(argv[0]);
    Crop item = CropUtils.getCropInfo(cid);
    UserPermission perm = UserPermission::NONE;
    if (uid == item.owner.uid) perm = UserPermission::OWNER;
    else {
        for (int i = 0; i < item.viewers.size(); i++)
            if (uid == item.viewers[i].uid) perm = UserPermission::VIEWER;
        for (int i = 0; i < item.editors.size(); i++)
            if (uid == item.editors[i].uid) perm = UserPermission::EDITOR;
    }
    
    if (perm == UserPermission::NONE) quickSendCode(401);
    if (perm != UserPermission::OWNER) item.viewers.clear(), item.editors.clear();

    std::string permission = getNameFromEnum(perm);

    quickSendData(200, item, permission);
};