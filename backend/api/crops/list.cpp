#include "../../ascas.h"

auto CropsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto get = getParam(request);
    std::string keyword = urldecode(get["keyword"]);
    UserPermission perm = get["perm"] == "" ? UserPermission::NONE : getEnumFromName(UserPermission, get["perm"]);
    CropSortOrder order = get["order"] == "" ? CropSortOrder::DEFAULT : getEnumFromName(CropSortOrder, get["order"]);
    std::vector<Crop> crops = CropUtils.listCrops(uid, keyword, perm, order);
    quickSendItems(200, packarr(crops));
};