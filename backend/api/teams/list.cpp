#include "../../ascas.h"

auto TeamsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto get = getParam(request);
    std::string keyword = urldecode(get["keyword"]);
    TeamSortOrder order = get["order"] == "" ? TeamSortOrder::DEFAULT : getEnumFromName(TeamSortOrder, get["order"]);
    std::vector<Team> crops = TeamUtils.listTeams(uid,keyword,order);
    quickSendItems(200, packarr(crops));
};