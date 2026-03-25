#pragma once

#include <string>
#include "model.cpp"
#include "user.cpp"

class TeamBase {
    public:

    int tid = 0;
    std::string title = "";
    std::string description = "";
    User owner;
    time_t createdAt = 0;
    std::vector<User> members = {};

    static TeamBase fromJsonObject(Json::Value obj) {
        return TeamBase({
            .tid = obj["tid"].asInt(),
            .title = obj["title"].asString(),
            .description = obj["description"].asString(),
            .owner = obj["owner"].as<User>(),
            .createdAt = obj["createdAt"].asInt64(),
            .members = extarr<User>(obj["members"])
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["tid"] = tid;
        res["title"] = title;
        res["description"] = description;
        res["owner"] = owner;
        res["createdAt"] = createdAt;
        res["members"] = packarr(members);
        return res;
    }
};
initModel(Team);

enum class TeamSortOrder {
    DEFAULT,
    NAME,
    CREATEDAT
};
initEnum(TeamSortOrder, DEFAULT, CREATEDAT);