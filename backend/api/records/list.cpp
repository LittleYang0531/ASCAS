#include "../../ascas.h"

auto CropsRecordsList = [](client_conn conn, http_request request, param argv) {
    if(request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
    Crop crop = CropUtils.getCropInfo(stoi(argv[0]), uid);
    auto posts = json_decode(request.postdata);
    WhereNode where = WhereNode::fromJsonObject(posts["where"]);
    std::vector<OrderNode> order = extarr<OrderNode>(posts["order"]);
    int limit = posts["limit"].asInt();
    int offset = posts["offset"].asInt();
    int count = 0;
    Json::Value items = packarr(RecordUtils.list(crop, where, order, limit, offset, count));
    if (count == -1) quickSendCode(404);
    if (count == -2) quickSendCode(400);
    int pageCount = (count + limit - 1) / limit;
    quickSendData(200, pageCount, items);
};