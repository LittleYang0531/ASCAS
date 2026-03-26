#pragma once

#include "model.cpp"
#include "user.cpp"
#include <string>

class MessageBase {
    public:

    int mid = 0;
    User user;
    std::string message = "";
    time_t createdAt = 0;

    static MessageBase fromJsonObject(Json::Value obj) {
        return MessageBase({
            .mid = obj["mid"].asInt(),
            .user = obj["user"].as<User>(),
            .message = obj["message"].asString(),
            .createdAt = obj["createdAt"].asInt64()
        });
    }

    Json::Value toJsonObject() {
        Json::Value val;
        val["mid"] = mid;
        val["user"] = user;
        val["message"] = message;
        val["createdAt"] = createdAt;
        return val;
    }
};
initModel(Message);

class TalkBase {
    public:
    
    std::string title = "";
    std::string talkId = "";
    std::string avatar = "";
    int unread = 0;
    Message latest;

    static TalkBase fromJsonObject(Json::Value obj) {
        return TalkBase({
            .title = obj["title"].asString(),
            .talkId = obj["talkId"].asString(),
            .avatar = obj["avatar"].asString(),
            .unread = obj["unread"].asInt(),
            .latest = obj["latest"].as<Message>()
        });
    }

    Json::Value toJsonObject() {
        Json::Value val;
        val["title"] = title;
        val["talkId"] = talkId;
        val["avatar"] = avatar;
        val["unread"] = unread;
        val["latest"] = latest;
        return val;
    }
};
initModel(Talk);