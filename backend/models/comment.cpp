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

    static CommentBase fromJsonObject(Json::Value obj) {
        return CommentBase({
            .cid = obj["cid"].asInt(),
            .author = obj["author"].as<User>(),
            .comment = obj["comment"].asString(),
            .likes = obj["likes"].asInt(),
            .dislikes = obj["dislikes"].asInt()
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["cid"] = cid;
        res["author"] = author;
        res["comment"] = comment;
        res["likes"] = likes;
        res["dislikes"] = dislikes;
        return res;
    }
};
initModel(Comment);