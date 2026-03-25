#include "../../ascas.h"

auto MessagesList = [](client_conn conn, http_request request, param argv) {
    int uid = UserUtils.checkLogin(request);
    if (!uid) quickSendCode(401);
    auto list = MessageUtils.getTalkList(uid);
    int unread = MessageUtils.getUnread(uid);
    auto items = packarr(list);
    quickSendData(200, items, unread);
};