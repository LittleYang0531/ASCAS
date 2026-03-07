#include "../../ascas.h"

auto UsersSearch = [](client_conn conn, http_request request, param argv) {
    auto get = getParam(request);
    std::string keyword = get["keyword"];
    if (keyword == "") quickSendCode(400);
    std::vector<User> res = UserUtils.search(keyword);
    quickSendItems(200, packarr(res));
};