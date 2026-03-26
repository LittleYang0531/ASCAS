#pragma once
#include "../ascas.h"

class MessageUtils {
    private:

    std::string getUsersTalkId(int uid1, int uid2) {
        return "users-" + std::to_string(std::min(uid1, uid2)) + "-" + std::to_string(std::max(uid1, uid2));
    }
    std::string getTeamTalkId(int tid) {
        return "team-" + std::to_string(tid);
    }

    std::vector<Message> getMessages(std::string talkId, int maxmid = 2147483647) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT mid, uid, message, createdAt, A.name AS title FROM messages "
            "INNER JOIN users AS A ON A.id = uid "
            "WHERE talkId = \"%s\" AND mid <= %d "
            "ORDER BY mid DESC "
            "LIMIT 10",
            talkId.c_str(),
            maxmid
        );
        std::vector<Message> messages;
        for (int i = 0; i < res.size(); i++)
            messages.push_back(Message({
                .mid = stoi(res[i]["mid"]),
                .user = User({
                    .uid = stoi(res[i]["uid"]),
                    .name = res[i]["title"]
                }),
                .message = res[i]["message"],
                .createdAt = stoll(res[i]["createdAt"])
            }));
        return messages;
    }

    Message sendMessage(std::string talkId, int uid, std::string message) {
        quick_mysqli_connect();
        
        time_t curr = time(NULL);
        mysqli_execute(
            mysql,
            "INSERT INTO messages (talkId, uid, message, createdAt) VALUES (\"%s\", %d, \"%s\", %lld)",
            talkId.c_str(),
            uid,
            quote_encode(message).c_str(),
            curr
        );

        int mid = stoi(mysqli_query(
            mysql,
            "SELECT MAX(mid) AS id FROM messages WHERE talkId = \"%s\" AND uid = %d",
            talkId.c_str(),
            uid
        )[0]["id"]);
        return Message({
            .mid = mid,
            .user = User({ .uid = uid }),
            .message = message,
            .createdAt = curr
        });
    }

    public:

    std::vector<Talk> getTalkList(int uid) {
        quick_mysqli_connect();
        auto userTalks = mysqli_query(
            mysql,
            "SELECT * FROM user_talks WHERE uid1 = %d OR uid2 = %d",
            uid,
            uid
        );
        auto teams = mysqli_query(
            mysql,
            "SELECT A.tid FROM team_members AS A "
            "INNER JOIN team_talks AS B ON B.tid = A.tid "
            "WHERE A.uid = %d",
            uid
        );
        std::vector<std::string> talkIds;
        for (int i = 0; i < userTalks.size(); i++) 
            talkIds.push_back("\"" + getUsersTalkId(stoi(userTalks[i]["uid1"]), stoi(userTalks[i]["uid2"])) + "\"");
        for (int i = 0; i < teams.size(); i++)
            talkIds.push_back("\"" + getTeamTalkId(stoi(teams[i]["tid"])) + "\"");
        auto res = mysqli_query(
            mysql,
            "SELECT B.talkId, A.mid, B.uid, B.message, B.createdAt, C.name AS title "
            "FROM (SELECT MAX(mid) AS mid FROM messages GROUP BY talkId) A "
            "INNER JOIN messages AS B ON B.mid = A.mid "
            "INNER JOIN users AS C ON C.id = B.uid "
            "WHERE B.talkId IN (%s)",
            join(", ", talkIds).c_str()
        );
        auto unread = mysqli_query(
            mysql,
            "SELECT B.talkId, COUNT(A.mid) AS count FROM unread_marks AS A "
            "INNER JOIN messages AS B ON B.mid = A.mid "
            "WHERE A.uid = %d "
            "GROUP BY B.talkId",
            uid
        );

        auto userTitle = mysqli_query(
            mysql,
            "SELECT uid1, uid2, A.name AS title1, B.name AS title2 "
            "FROM user_talks "
            "INNER JOIN users AS A ON A.id = uid1 "
            "INNER JOIN users AS B on B.id = uid2 "
            "WHERE uid1 = %d OR uid2 = %d",
            uid,
            uid
        );
        auto teamTitle = mysqli_query(
            mysql,
            "SELECT A.tid, C.title "
            "FROM team_members AS A "
            "INNER JOIN team_talks AS B ON B.tid = A.tid "
            "INNER JOIN teams AS C ON C.id = A.tid "
            "WHERE A.uid = %d",
            uid
        );
        std::map<int, std::string> userTitles, teamTitles;
        for (int i = 0; i < userTitle.size(); i++) 
            userTitles[stoi(userTitle[i]["uid1"])] = userTitle[i]["title1"],
            userTitles[stoi(userTitle[i]["uid2"])] = userTitle[i]["title2"];
        for (int i = 0; i < teamTitle.size(); i++) 
            teamTitles[stoi(teamTitle[i]["tid"])] = teamTitle[i]["title"];

        std::map<std::string, int> unreadCounts;
        for (int i = 0; i < unread.size(); i++)
            unreadCounts[unread[i]["talkId"]] = stoi(unread[i]["count"]);

        std::vector<Talk> talks;
        for (int i = 0; i < res.size(); i++) {
            if (res[i]["talkId"].starts_with("users")) {
                std::vector<std::string> tmp = explode("-", res[i]["talkId"]);
                int uid1 = stoi(tmp[1]), uid2 = stoi(tmp[2]);
                talks.push_back(Talk({
                    .title = userTitles[uid1 != uid ? uid1 : uid2],
                    .talkId = res[i]["talkId"],
                    .avatar = "/users/" + std::to_string(uid1 != uid ? uid1 : uid2) + "/avatar",
                    .unread = unreadCounts[res[i]["talkId"]],
                    .latest = Message({
                        .mid = stoi(res[i]["mid"]),
                        .user = User({
                            .uid = (uid1 != uid ? uid1 : uid2),
                            .name = userTitles[uid1 != uid ? uid1 : uid2]
                        }),
                        .message = res[i]["message"],
                        .createdAt = stoll(res[i]["createdAt"])
                    })
                }));
            } else if (res[i]["talkId"].starts_with("team")) {
                std::vector<std::string> tmp = explode("-", res[i]["talkId"]);
                int tid = stoi(tmp[1]);
                talks.push_back(Talk({
                    .title = teamTitles[tid],
                    .talkId = res[i]["talkId"],
                    .avatar = "/users/generateAvatar?s=" + res[i]["talkId"],
                    .unread = unreadCounts[res[i]["talkId"]],
                    .latest = Message({
                        .mid = stoi(res[i]["mid"]),
                        .user = User({
                            .uid = stoi(res[i]["uid"]),
                            .name = res[i]["title"]
                        }),
                        .message = res[i]["message"],
                        .createdAt = stoll(res[i]["createdAt"])
                    })
                }));
            }
        }
        return talks;
    }

    int getUnread(int uid) {
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM unread_marks WHERE uid = %d",
            uid
        )[0]["count"]);
    }

    Message sendUserMessage(int fromuid, int touid, std::string message) {
        quick_mysqli_connect();
        int count = stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM user_talks WHERE uid1 = %d AND uid2 = %d",
            std::min(fromuid, touid),
            std::max(fromuid, touid)
        )[0]["count"]);
        if (!count) {
            mysqli_execute(
                mysql,
                "INSERT INTO user_talks VALUES (%d, %d)",
                std::min(fromuid, touid),
                std::max(fromuid, touid)
            );
        }

        std::string talkId = getUsersTalkId(fromuid, touid);
        Message msg = sendMessage(talkId, fromuid, message);

        mysqli_execute(
            mysql,
            "INSERT INTO unread_marks VALUES (%d, %d)",
            msg.mid,
            touid
        );
        return msg;
    }

    Message sendTeamMessage(int uid, int tid, std::string message) {
        std::string talkId = getTeamTalkId(tid);
        Message msg = sendMessage(talkId, uid, message);

        mysqli_execute(
            mysql,
            "INSERT INTO unread_marks SELECT %d, uid FROM team_members WHERE uid != %d",
            msg.mid,
            uid
        );
        return msg;
    }

    std::vector<Message> getUsersMessages(int fromuid, int touid, int maxmid = 2147483647) {
        quick_mysqli_connect();
        std::string talkId = getUsersTalkId(fromuid, touid);
        mysqli_execute(
            mysql,
            "DELETE A FROM unread_marks AS A "
            "INNER JOIN messages AS B ON B.mid = A.mid "
            "WHERE B.talkId = \"%s\" AND A.uid = %d",
            talkId.c_str(),
            fromuid
        );

        return getMessages(talkId, maxmid);
    }

    std::vector<Message> getTeamMessages(int tid, int maxmid = 2147483647) {
        std::string talkId = getTeamTalkId(tid);
        return getMessages(talkId, maxmid);
    }
}MessageUtils;