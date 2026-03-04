#pragma once

#include "enum.h"
#include "model.cpp"
#include "user.cpp"
#include <jsoncpp/json/value.h>
#include <string>
#include <vector>

enum class UserPermission {
    NONE,
    VIEWER,
    EDITOR,
    OWNER
};
initEnum(UserPermission, NONE, OWNER);

enum class RecordPropertyType {
    NUMBER,
    STRING,
    IMAGE
};
initEnum(RecordPropertyType, NUMBER, IMAGE);

class RecordPropertyBase {
    public:

    std::string name = ""; // 属性编号，由系统自动生成
    std::string title = "";
    std::string unit = "";
    bool required = true;
    std::string def = "";
    RecordPropertyType type = RecordPropertyType::NUMBER;

    static RecordPropertyBase fromJsonObject(Json::Value obj) {
        return RecordPropertyBase({
            .name = obj["name"].asString(),
            .title = obj["title"].asString(),
            .unit = obj["unit"].asString(),
            .required = obj["required"].asBool(),
            .def = obj["def"].asString(),
            .type = getEnumFromName(RecordPropertyType, obj["type"].asString())
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["title"] = title;
        res["unit"] = unit;
        res["required"] = required;
        res["def"] = def;
        res["type"] = getNameFromEnum(type);
        return res;
    }
};
initModel(RecordProperty);

class CropBase {
    public:

    int cid = 0;
    std::string name = ""; // 作物编号，由系统自动生成
    std::string title = ""; 
    std::string description = "";
    std::vector<RecordProperty> properties = {};
    User owner = User({});
    std::vector<User> editors = {};
    std::vector<User> viewers = {};
    time_t createdAt = 0;
    UserPermission permission = UserPermission::NONE;

    std::vector<int> vals;

    static CropBase fromJsonObject(Json::Value obj) {
        return CropBase({
            .cid = obj["cid"].asInt(),
            .name = obj["name"].asString(),
            .title = obj["title"].asString(),
            .description = obj["description"].asString(),
            .properties = extarr<RecordProperty>(obj["properties"]),
            .owner = obj["user"].as<User>(),
            .editors = extarr<User>(obj["editors"]),
            .viewers = extarr<User>(obj["viewers"]),
            .createdAt = obj["createdAt"].asInt64(),
            .permission = getEnumFromName(UserPermission, obj["permission"].asString())
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["cid"] = cid;
        res["name"] = name;
        res["title"] = title;
        res["description"] = description;
        res["properties"] = packarr(properties);
        res["owner"] = owner;
        res["editors"] = packarr(editors);
        res["viewers"] = packarr(viewers);
        res["createdAt"] = createdAt;
        res["permission"] = getNameFromEnum(permission);
        return res;
    }
};
initModel(Crop);