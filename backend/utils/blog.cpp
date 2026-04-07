#pragma once
#include "../ascas.h"

class BlogUtils {
    public:

    std::vector<Blog> getBlogList(int uid, std::string keyword, BlogSortOrder sort) {
        quick_mysqli_connect();

        std::string order = "";
        switch (sort) {
            case BlogSortOrder::DEFAULT:
            case BlogSortOrder::TITLE:
                order = "A.title ASC"; break;
            case BlogSortOrder::CREATEDAT:
                order = "A.createdAt DESC"; break;
            case BlogSortOrder::LIKES:
                order = "B.count DESC"; break;
            case BlogSortOrder::COMMENTS:
                order = "E.count DESC"; break;
            case BlogSortOrder::STARS:
                order = "D.count DESC"; break;
            default:
                order = "A.title ASC"; break;
        }

        auto res = mysqli_query(
            mysql,
            "SELECT A.*, A.id AS bid, B.count AS likes, C.count AS dislikes, D.count AS stars, E.count AS comments, F.* FROM blogs AS A "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_likes GROUP BY bid) AS B ON B.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_dislikes GROUP BY bid) AS C ON C.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_stars GROUP BY bid) AS D ON D.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_comments GROUP BY bid) AS E ON E.bid = A.id "
            "LEFT JOIN users AS F ON F.id = A.uid "
            "WHERE A.uid = %d AND (A.title LIKE \"%%%s%%\" OR A.content LIKE \"%%%s%%\") "
            "ORDER BY %s",
            uid,
            quote_encode(keyword).c_str(),
            quote_encode(keyword).c_str(),
            order.c_str()
        );

        std::vector<Blog> ans = {};
        for (int i = 0; i < res.size(); i++) ans.push_back(Blog({
            .bid = stoi(res[i]["bid"]),
            .author = User({
                .uid = stoi(res[i]["uid"]),
                .name = res[i]["name"],
                .email = res[i]["email"],
                .isAdmin = stoi(res[i]["isAdmin"]) ? true : false
            }),
            .title = res[i]["title"],
            .content = res[i]["content"],
            .createdAt = stoll(res[i]["createdAt"]),
            .likes = stoi(res[i]["likes"] == "" ? "0" : res[i]["likes"]),
            .dislikes = stoi(res[i]["dislikes"] == "" ? "0" : res[i]["dislikes"]),
            .stars = stoi(res[i]["stars"] == "" ? "0" : res[i]["stars"]),
            .comments = stoi(res[i]["comments"] == "" ? "0" : res[i]["comments"])
        }));

        return ans;
    }

    int exists(int bid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blogs WHERE id = %d",
            bid
        )[0]["count"]);
    }

    Blog getBlogInfo(int bid) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT A.*, A.id AS bid, B.count AS likes, C.count AS dislikes, D.count AS stars, E.count AS comments, F.* FROM blogs AS A "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_likes GROUP BY bid) AS B ON B.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_dislikes GROUP BY bid) AS C ON C.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_stars GROUP BY bid) AS D ON D.bid = A.id "
            "LEFT JOIN (SELECT bid, COUNT(*) AS count FROM blog_comments GROUP BY bid) AS E ON E.bid = A.id "
            "LEFT JOIN users AS F ON F.id = A.uid "
            "WHERE A.id = %d",
            bid
        )[0];

        return Blog({
            .bid = stoi(res["bid"]),
            .author = User({
                .uid = stoi(res["uid"]),
                .name = res["name"],
                .email = res["email"],
                .isAdmin = stoi(res["isAdmin"]) ? true : false
            }),
            .title = res["title"],
            .content = res["content"],
            .createdAt = stoll(res["createdAt"]),
            .likes = stoi(res["likes"] == "" ? "0" : res["likes"]),
            .dislikes = stoi(res["dislikes"] == "" ? "0" : res["dislikes"]),
            .stars = stoi(res["stars"] == "" ? "0" : res["stars"]),
            .comments = stoi(res["comments"] == "" ? "0" : res["comments"])
        });
    }

    int checkLike(int uid, int bid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_likes WHERE bid = %d AND uid = %d",
            bid,
            uid
        )[0]["count"]);
    }

    int checkDislike(int uid, int bid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_dislikes WHERE bid = %d AND uid = %d",
            bid,
            uid
        )[0]["count"]);
    }

    int checkStar(int uid, int bid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM blog_stars WHERE bid = %d AND uid = %d",
            bid,
            uid
        )[0]["count"]);
    }

    int create(int uid, Blog blog) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "INSERT INTO blogs (uid, title, content, createdAt) VALUES (%d, \"%s\", \"%s\", %lld)",
            uid,
            quote_encode(blog.title).c_str(),
            quote_encode(blog.content).c_str(),
            time(NULL)
        );

        return stoi(mysqli_query(
            mysql,
            "SELECT MAX(id) AS maxid FROM blogs WHERE uid = %d",
            uid
        )[0]["maxid"]);
    }

    void edit(Blog blog) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "UPDATE blogs SET title = \"%s\", content = \"%s\" WHERE id = %d",
            quote_encode(blog.title).c_str(),
            quote_encode(blog.content).c_str(),
            blog.bid
        );
    }

    void remove(int bid) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "DELETE FROM blogs WHERE id = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM blog_likes WHERE bid = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM blog_dislikes WHERE bid = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM blog_stars WHERE bid = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE A FROM blog_comment_likes AS A "
            "INNER JOIN blog_comments AS B ON B.id = A.cid ",
            "WHERE B.bid = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE A FROM blog_comment_dislikes AS A "
            "INNER JOIN blog_comments AS B ON B.id = A.cid ",
            "WHERE B.bid = %d",
            bid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM blog_comments WHERE bid = %d",
            bid
        );
    }

    void like(int uid, int bid) {
        quick_mysqli_connect();

        bool liked = checkLike(uid, bid);
        if (liked) {
            mysqli_execute(
                mysql,
                "DELETE FROM blog_likes WHERE bid = %d AND uid = %d",
                bid,
                uid
            );
        } else {
            if (checkDislike(uid, bid)) {
                mysqli_execute(
                    mysql,
                    "DELETE FROM blog_dislikes WHERE bid = %d AND uid = %d",
                    bid,
                    uid
                );
            }
            mysqli_execute(
                mysql,
                "INSERT INTO blog_likes (bid, uid) VALUES (%d, %d)",
                bid,
                uid
            );
        }
    }

    void dislike(int uid, int bid) {
        quick_mysqli_connect();

        bool disliked = checkDislike(uid, bid);
        if (disliked) {
            mysqli_execute(
                mysql,
                "DELETE FROM blog_dislikes WHERE bid = %d AND uid = %d",
                bid,
                uid
            );
        } else {
            if (checkLike(uid, bid)) {
                mysqli_execute(
                    mysql,
                    "DELETE FROM blog_likes WHERE bid = %d AND uid = %d",
                    bid,
                    uid
                );
            }
            mysqli_execute(
                mysql,
                "INSERT INTO blog_dislikes (bid, uid) VALUES (%d, %d)",
                bid,
                uid
            );
        }
    }

    void star(int uid, int bid) {
        quick_mysqli_connect();

        bool stared = checkStar(uid, bid);
        if (stared) {
            mysqli_execute(
                mysql,
                "DELETE FROM blog_stars WHERE bid = %d AND uid = %d",
                bid,
                uid
            );
        } else {
            mysqli_execute(
                mysql,
                "INSERT INTO blog_stars (bid, uid) VALUES (%d, %d)",
                bid,
                uid
            );
        }
    }
}BlogUtils;