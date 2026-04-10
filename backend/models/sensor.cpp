#pragma once

#include "model.cpp"
#include "user.cpp"

class SensorBase {
    public:

    int sid;
    User owner;
    std::string title;
    std::string unit;
    std::vector<double> data;
    std::vector<time_t> createdAt;

    static SensorBase fromJsonObject(Json::Value obj) {
        return SensorBase({
            .sid = obj["sid"].asInt(),
            .owner = obj["owner"].as<User>(),
            .title = obj["title"].asString(),
            .unit = obj["unit"].asString(),
            .data = extarr<double>(obj["data"]),
            .createdAt = extarr<time_t>(obj["createdAt"], [](Json::Value obj){ return obj.asInt64(); })
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["sid"] = sid;
        res["owner"] = owner;
        res["title"] = title;
        res["unit"] = unit;
        res["data"] = packarr(data);
        res["createdAt"] = packarr(createdAt);
        return res;
    }
};
initModel(Sensor);