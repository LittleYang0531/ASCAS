#include "../ascas.h"

auto Overview = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    int crop_count = CropUtils.getCropsCount();
    int record_count = RecordUtils.getRecordCount();
    int user_count = UserUtils.searchInTeams(uid, "").size();
    std::vector<Crop> tmp = CropUtils.listCrops(uid, "", UserPermission::NONE, CropSortOrder::DEFAULT);
    while (tmp.size() && tmp.back().permission == UserPermission::VIEWER) tmp.pop_back();
    Json::Value crops = packarr(tmp);
    quickSendData(200, crop_count, record_count, user_count, crops);
};