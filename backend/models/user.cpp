#pragma once

#include "model.cpp"
#include <jsoncpp/json/value.h>
#include <string>

class UserBase {
    public:

    int uid = 0;
    std::string name = "";
    std::string email = "";
    bool isAdmin = false;

    static UserBase fromJsonObject(Json::Value obj) {
        return UserBase({
            .uid = obj["uid"].asInt(),
            .name = obj["name"].asString(),
            .email = obj["email"].asString(),
            .isAdmin = obj["isAdmin"].asBool()
        });
    } 

    Json::Value toJsonObject() {
        Json::Value res;
        res["uid"] = uid;
        res["name"] = name;
        res["email"] = email;
        res["isAdmin"] = isAdmin;
        return res;
    }
};
initModel(User);