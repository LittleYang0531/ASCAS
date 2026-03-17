#pragma once
#include "../ascas.h"

class RecordUtils {
private:
    const std::string sessionStrings =
        "0123456789abcdef";
    std::string generateSession(int len = 64) {
        srand(clock3());
        std::string session = "";
        for (int i = 0; i < len; i++)
            session += sessionStrings[rand() % sessionStrings.size()];
        return session;
    }

    int checkWhereNode(WhereNodeBase node, std::map<std::string, RecordPropertyType> types) {
        if (node.isLeaf) {
            std::string realColumn = node.column;
            if (!std::set<std::string>({ "id", "name", "uid" }).count(realColumn) && types.count(realColumn) == 0) return -1;
            RecordPropertyType type = types[node.column];
            if (type == RecordPropertyType::NUMBER && SQLOperator::LIKE <= node.op) return -2;
            if (type != RecordPropertyType::NUMBER && SQLOperator::GREATER <= node.op && node.op <= SQLOperator::SMALLER_OR_EQUAL) return -2;
            return 0;
        } else {
            for (int i = 0; i < node.params.size(); i++) {
                int ret = checkWhereNode(node.params[i], types);
                if (ret != 0) return ret;
            }
            return 0;
        }
    }
    std::string whereNodeToString(WhereNodeBase node, std::map<std::string, RecordPropertyType> types) {
        if (node.isLeaf) {
            std::string realColumn = node.column;
            if (!std::set<std::string>({ "id", "name", "uid" }).count(realColumn)) realColumn = "var_" + realColumn;
            std::string value = node.value;
            if (types[node.column] != RecordPropertyType::NUMBER) value = "\"" + quote_encode(value) + "\"";
            else value = std::to_string(stod(value));
            return realColumn + " " + SQLOperators[int(node.op)] + " " + value;
        } else {
            std::vector<std::string> exprs;
            for (int i = 0; i < node.params.size(); i++) {
                std::string expr = whereNodeToString(node.params[i], types);
                exprs.push_back(expr);
            }
            if (exprs.size() == 0) return "(true)";
            return "(" + join(node.isAnd ? " AND " : " OR ", exprs) + ")";
        }
    }

public:
    std::vector<Json::Value> list(Crop crop, WhereNode where, std::vector<OrderNode> order, int limit, int offset, int &count) {
        quick_mysqli_connect();
        std::map<std::string, RecordPropertyType> types;
        for (int i = 0; i < crop.properties.size(); i++) {
            std::string name = crop.properties[i].name;
            RecordPropertyType type = crop.properties[i].type;
            types[name] = type;
        }
        types["id"] = RecordPropertyType::NUMBER;
        types["name"] = RecordPropertyType::STRING;
        types["uid"] = RecordPropertyType::NUMBER;
        int check = checkWhereNode(where, types);
        if (check != 0) {
            count = check;
            return {};
        }
        std::string whereString = whereNodeToString(where, types);

        std::vector<std::string> orderExprs;
        for (int i = 0; i < order.size(); i++) {
            std::string realColumn = order[i].column;
            if (!std::set<std::string>({ "id", "name", "uid" }).count(realColumn)) realColumn = "var_" + realColumn;
            if (!types.count(order[i].column)) {
                count = -1;
                return {};
            }
            orderExprs.push_back(realColumn + " " + (order[i].isASC ? "ASC" : "DESC"));
        }
        std::string orderString = join(", ", orderExprs);
        if (orderString != "") orderString = "ORDER BY " + orderString;

        count = stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM table_%s WHERE %s",
            crop.name.c_str(),
            whereString.c_str()
        )[0]["count"]);

        auto res = mysqli_query(
            mysql,
            "SELECT * FROM table_%s WHERE %s %s LIMIT %d OFFSET %d",
            crop.name.c_str(),
            whereString.c_str(),
            orderString.c_str(),
            limit,
            offset
        );
        std::vector<Json::Value> vec;
        for (int i = 0; i < res.size(); i++) {
            Json::Value tmp;
            for (auto v : res[i]) {
                std::string column = v.first;
                std::string value = v.second;
                std::string realColumn = column;
                if (realColumn.starts_with("var_")) realColumn = realColumn.substr(4);
                RecordPropertyType type = types[realColumn];
                if (type != RecordPropertyType::NUMBER) tmp[realColumn] = value;
                else tmp[realColumn] = stod(value);
            }
            vec.push_back(tmp);
        }
        return vec;
    }

    void add(Crop crop, Json::Value posts, int uid) {
        quick_mysqli_connect();
        std::string all, sep = ",", c = "\"",var;
        size_t n = crop.properties.size();
        std::vector<std::string> vjoin,k;
        for (int i = 0; i < n; ++i)
        {
            std::string newline;
            if (crop.properties[i].type == RecordPropertyType::NUMBER)
            {
                newline += std::to_string(stod((posts[crop.properties[i].name].asString())));
            }
            else
            {
                newline += c;
                newline += quote_encode(posts[crop.properties[i].name].asString());
                newline += c;
            }
            k.push_back("var_" + crop.properties[i].name);
            vjoin.push_back(newline);
        }
        all = join(sep, vjoin);
        var = join(sep, k);
        std::string name = c + generateSession(16) + c;
        mysqli_execute(
            mysql,
            "insert into table_%s(name,uid,%s) values(%s,%d,%s)",
            quote_encode(crop.name).c_str(),
            quote_encode(var).c_str(),
            name.c_str(),
            uid,
            all.c_str()
        );
    }
}RecordUtils;