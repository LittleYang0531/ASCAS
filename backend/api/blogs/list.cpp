#include "../../ascas.h"

auto BlogsList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto gets = getParam(request);
    std::string keyword = gets["keyword"];
    BlogSortOrder order = gets["order"] == "" ? BlogSortOrder::DEFAULT : getEnumFromName(BlogSortOrder, gets["order"]);
    auto list = BlogUtils.getBlogList(uid, keyword, order);
    quickSendItems(200, packarr(list));
};