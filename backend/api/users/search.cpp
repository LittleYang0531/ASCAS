#include "../../ascas.h"

auto UsersSearch = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto get = getParam(request);
    std::string keyword = get["keyword"];
    bool searchAll = get.find("searchAll") != get.end();
    if (keyword == "") quickSendCode(400);
    std::vector<User> res = !searchAll ? UserUtils.searchInTeams(uid, keyword) : UserUtils.search(keyword);
    quickSendItems(200, packarr(res));
};