#pragma once

#include <jsoncpp/json/value.h>
#include <string>

class User {
    public:

    int uid;
    std::string name;
    std::string email;
    bool isAdmin;

    Json::Value toJsonObject() {
        Json::Value res;
        res["uid"] = uid;
        res["name"] = name;
        res["email"] = email;
        res["isAdmin"] = isAdmin;
        return res;
    }
};