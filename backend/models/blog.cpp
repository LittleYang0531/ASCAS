#pragma once

#include "model.cpp"
#include "user.cpp"
#include <jsoncpp/json/value.h>
#include <string>

enum class BlogSortOrder {
    DEFAULT,
    TITLE,
    CREATEDAT,
    LIKES,
    COMMENTS,
    STARS
};  
initEnum(BlogSortOrder, DEFAULT, STARS);

class BlogBase {
    public:

    int bid;
    User author;
    std::string title;
    std::string content;
    time_t createdAt;
    int likes;
    int dislikes;
    int stars;
    int comments;

    static BlogBase fromJsonObject(Json::Value obj) {
        return BlogBase({
            .bid = obj["bid"].asInt(),
            .author = obj["author"].as<User>(),
            .title = obj["title"].asString(),
            .content = obj["content"].asString(),
            .createdAt = obj["createdAt"].asInt64(),
            .likes = obj["likes"].asInt(),
            .dislikes = obj["dislikes"].asInt(),
            .stars = obj["stars"].asInt(),
            .comments = obj["comments"].asInt()
        });
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["bid"] = bid;
        res["author"] = author;
        res["title"] = title;
        res["content"] = content;
        res["createdAt"] = createdAt;
        res["likes"] = likes;
        res["dislikes"] = dislikes;
        res["stars"] = stars;
        res["comments"] = comments;
        return res;
    }
};
initModel(Blog);