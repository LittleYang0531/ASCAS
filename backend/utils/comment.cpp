#pragma once

#include "../ascas.h"

class CommentUtils {
    public:

    int send(int bid, int uid, std::string msg) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "INSERT INTO blog_comments (bid, uid, comment, createdAt) VALUES (%d, %d, \"%s\", %lld)",
            bid,
            uid,
            quote_encode(msg).c_str(),
            time(NULL)
        );

        return stoi(mysqli_query(
            mysql,
            "SELECT MAX(id) AS maxid FROM blog_comments WHERE bid = %d AND uid = %d",
            bid,
            uid
        )[0]["maxid"]);
    }

    std::vector<Comment> list(int uid, int bid, int maxcid = INT_MAX) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT A.*, A.id AS cid, B.*, C.count AS likes, D.count AS dislikes, E.count AS liked, F.count AS disliked FROM blog_comments AS A "
            "INNER JOIN users AS B ON B.id = A.uid "
            "LEFT JOIN (SELECT cid, COUNT(*) AS count FROM blog_comment_likes GROUP BY cid) AS C ON C.cid = A.id "
            "LEFT JOIN (SELECT cid, COUNT(*) AS count FROM blog_comment_dislikes GROUP BY cid) AS D ON D.cid = A.id "
            "LEFT JOIN (SELECT cid, COUNT(*) AS count FROM blog_comment_likes WHERE uid = %d GROUP BY cid) AS E ON E.cid = A.id "
            "LEFT JOIN (SELECT cid, COUNT(*) AS count FROM blog_comment_dislikes WHERE uid = %d GROUP BY cid) AS F ON F.cid = A.id "
            "WHERE A.bid = %d AND A.id <= %d "
            "ORDER BY A.id DESC "
            "LIMIT 10",
            uid,
            uid,
            bid,
            maxcid
        );

        std::vector<Comment> ans = {};
        for (int i = 0; i < res.size(); i++) ans.push_back(Comment({
            .cid = stoi(res[i]["cid"]),
            .author = User({
                .uid = stoi(res[i]["uid"]),
                .name = res[i]["name"],
                .email = res[i]["email"],
                .isAdmin = stoi(res[i]["isAdmin"]) ? true : false
            }),
            .comment = res[i]["comment"],
            .likes = res[i]["likes"] == "" ? 0 : stoi(res[i]["likes"]),
            .dislikes = res[i]["dislikes"] == "" ? 0 : stoi(res[i]["dislikes"]),
            .liked = res[i]["liked"] == "" ? false : stoi(res[i]["liked"]) ? true : false,
            .disliked = res[i]["disliked"] == "" ? false : stoi(res[i]["disliked"]) ? true : false,
            .createdAt = stoll(res[i]["createdAt"])
        }));

        return ans;
    }

    User getAuthor(int cid) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT B.* FROM blog_comments AS A "
            "INNER JOIN users AS B ON B.id = A.uid "
            "WHERE A.id = %d",
            cid
        );

        return User({
            .uid = stoi(res[0]["id"]),
            .name = res[0]["name"],
            .email = res[0]["email"],
            .isAdmin = stoi(res[0]["isAdmin"]) ? true : false
        });
    }

    int exists(int cid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_comments WHERE id = %d",
            cid
        )[0]["count"]);
    }

    int checkLike(int uid, int cid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_comment_likes WHERE cid = %d AND uid = %d",
            cid,
            uid
        )[0]["count"]);
    }

    int checkDislike(int uid, int cid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_comment_dislikes WHERE cid = %d AND uid = %d",
            cid,
            uid
        )[0]["count"]);
    }

    void like(int uid, int cid) {
        quick_mysqli_connect();

        bool liked = checkLike(uid, cid);
        if (liked) {
            mysqli_execute(
                mysql,
                "DELETE FROM blog_comment_likes WHERE cid = %d AND uid = %d",
                cid,
                uid
            );
        } else {
            if (checkDislike(uid, cid)) {
                mysqli_execute(
                    mysql,
                    "DELETE FROM blog_comment_dislikes WHERE cid = %d AND uid = %d",
                    cid,
                    uid
                );
            }
            mysqli_execute(
                mysql,
                "INSERT INTO blog_comment_likes (cid, uid) VALUES (%d, %d)",
                cid,
                uid
            );
        }
    }

    void dislike(int uid, int cid) {
        quick_mysqli_connect();

        bool disliked = checkDislike(uid, cid);
        if (disliked) {
            mysqli_execute(
                mysql,
                "DELETE FROM blog_comment_dislikes WHERE cid = %d AND uid = %d",
                cid,
                uid
            );
        } else {
            if (checkLike(uid, cid)) {
                mysqli_execute(
                    mysql,
                    "DELETE FROM blog_comment_likes WHERE cid = %d AND uid = %d",
                    cid,
                    uid
                );
            }
            mysqli_execute(
                mysql,
                "INSERT INTO blog_comment_dislikes (cid, uid) VALUES (%d, %d)",
                cid,
                uid
            );
        }
    }
}CommentUtils;