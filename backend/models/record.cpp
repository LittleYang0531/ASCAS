#pragma once

#include "model.cpp"
enum class SQLOperator {
    // 通用筛选
    EQUAL, // 等于
    NOTEQUAL, // 不等于
    // 数值筛选
    GREATER, // 大于
    GREATER_OR_EQUAL, // 大于等于
    SMALLER, // 小于
    SMALLER_OR_EQUAL, // 小于等于
    // 字符串筛选
    LIKE, // like
    NOTLIKE, // notlike
    REGEXP, // regexp
    NOTREGEXP, // !regexp
};
initEnum(SQLOperator,EQUAL,NOTREGEXP);

const std::string SQLOperators[] = {
    "=",
    "!=",
    ">",
    ">=",
    "<",
    "<=",
    "LIKE",
    "NOT LIKE",
    "REGEXP",
    "NOT REGEXP"
};

class WhereNodeBase {
    public:

    bool isLeaf; // 是否为叶子节点
    std::string column; // 列名
    SQLOperator op; // 操作符
    std::string value; // 比较值
    bool isAnd; // 是否使用 AND 连接
    std::vector<WhereNodeBase> params; // 连接参数

    static WhereNodeBase fromJsonObject(Json::Value obj) {
        bool isLeaf = obj["isLeaf"].asBool();
        if (isLeaf) return WhereNodeBase({
            .isLeaf = true,
            .column = obj["column"].asString(),
            .op = getEnumFromName(SQLOperator, obj["op"].asString()),
            .value = obj["value"].asString()
        });
        else return WhereNodeBase({
            .isLeaf = false,
            .isAnd = obj["isAnd"].asBool(),
            .params = extarr<WhereNodeBase>(obj["params"], [](Json::Value obj){ return WhereNodeBase::fromJsonObject(obj); })
        }); 
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["isLeaf"] = isLeaf;
        if (isLeaf) {
            res["column"] = column;
            res["op"] = getNameFromEnum(op);
            res["value"] = value;
        } else {
            res["isAnd"] = isAnd;
            res["params"] = packarr(params, [](WhereNodeBase obj){ return obj.toJsonObject(); });
        }
        return res;
    }
};
initModel(WhereNode);

class OrderNodeBase {
    public:

    std::string column; // 列名
    bool isASC; // 是否为正序

    static OrderNodeBase fromJsonObject(Json::Value obj) {
        return OrderNodeBase({
            .column = obj["column"].asString(),
            .isASC = obj["isASC"].asBool()
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["column"] = column;
        res["isASC"] = isASC;
        return res;
    }
};
initModel(OrderNode);