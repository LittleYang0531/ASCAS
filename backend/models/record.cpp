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