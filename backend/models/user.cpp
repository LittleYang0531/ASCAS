#pragma once

#include "model.cpp"
#include <jsoncpp/json/value.h>
#include <string>

enum class UserPermission {
    NONE,
    VIEWER,
    EDITOR,
    OWNER
};
initEnum(UserPermission, NONE, OWNER);

class UserBase {
    public:

    int uid;
    std::string name;
    std::string email;
    bool isAdmin;

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