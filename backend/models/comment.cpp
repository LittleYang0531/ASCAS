#pragma once

#include "model.cpp"
#include "user.cpp"
#include <jsoncpp/json/value.h>
#include <string>

class CommentBase {
    public:

    int cid;
    User author;
    std::string comment;
    int likes;
    int dislikes;
    bool liked;
    bool disliked;
    time_t createdAt;

    static CommentBase fromJsonObject(Json::Value obj) {
        return CommentBase({
            .cid = obj["cid"].asInt(),
            .author = obj["author"].as<User>(),
            .comment = obj["comment"].asString(),
            .likes = obj["likes"].asInt(),
            .dislikes = obj["dislikes"].asInt(),
            .liked = obj["liked"].asBool(),
            .disliked = obj["disliked"].asBool(),
            .createdAt = obj["createdAt"].asInt64()
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["cid"] = cid;
        res["author"] = author;
        res["comment"] = comment;
        res["likes"] = likes;
        res["dislikes"] = dislikes;
        res["liked"] = liked;
        res["disliked"] = disliked;
        res["createdAt"] = createdAt;
        return res;
    }
};
initModel(Comment);