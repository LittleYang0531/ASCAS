#include "../../ascas.h"

auto CropsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    std::vector<Crop> crops = CropUtils.listCrops(uid);
    quickSendItems(200, packarr(crops));
};