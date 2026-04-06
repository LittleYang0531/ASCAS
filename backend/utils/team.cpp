#pragma once
#include "../ascas.h"

class TeamUtils {
    private:
    const std::string sessionStrings = 
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string generateSession(int len = 64) {
        srand(clock3());
        std::string session = "";
        for (int i = 0; i < len; i++) session += sessionStrings[rand() % sessionStrings.size()];
        return session;
    }

    public:
    
    // 检查团队是否存在
    int exists(int tid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM teams WHERE id = %d",
            tid
        )[0]["count"]);
    }

    bool inTeam(int uid, int tid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM team_members WHERE tid = %d AND uid = %d",
            tid,
            uid
        )[0]["count"]);
    }

    // 获取团队信息
    Team getTeamsInfo(int tid) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "select *from teams where id = %d",
            tid
        );
        MyAssert(
            res.size(),
            "tid %d not found, please use TeamUtils.exists(%d) to check whether cid %d is exists before calling this method.",
            tid, tid, tid
        );
        auto team = res[0];
        auto mem = mysqli_query(
            mysql,
            "select * from team_members where tid = %d",
            tid
        );
        std::vector<User> members;
        for(size_t i = 0;i < mem.size();++i)
        {
            members.push_back(UserUtils.getUserInfo(stoi(mem[i]["uid"])));
        }
        Team item = Team({
            .tid = stoi(team["id"]),
            .title = team["title"],
            .description = team["description"],
            .owner = UserUtils.getUserInfo(stoi(team["owner"])),
            .createdAt = stoll(team["createdAt"]),
            .members = members,
        });

        return item;
    }

    // 创建团队
    int create(Team team) {
        quick_mysqli_connect();
        mysqli_execute(
            mysql,
            "INSERT INTO teams (title, description, owner, createdAt) "
            "VALUES (\"%s\", \"%s\", %d,  %lld)",
            quote_encode(team.title).c_str(),
            quote_encode(team.description).c_str(),
            team.owner.uid,
            time(NULL)
        );

        int tid = stoi(mysqli_query(
            mysql,
            "SELECT max(id) as id FROM teams"
        )[0]["id"]);
        auto members = team.members;
        if(members.size() == 0)
        {
            members.push_back(
                User({
                    .uid = team.owner.uid,
                })
            );
        }
        std::vector<std::string> v;
        for(size_t i = 0;i < members.size();++i)
        {
            v.push_back("(" + std::to_string(tid) + "," + std::to_string(members[i].uid) + ")");
        }
        std::string ins = "values" + join(",",v);
        mysqli_execute(
            mysql,
            "insert into team_members(tid,uid) %s",
             ins.c_str()
        );
       return tid;
    }

     // 列举相关团队信息
    std::vector<Team> listTeams(int uid, std::string keyword, TeamSortOrder order) {
        quick_mysqli_connect();
        std::vector<Team> res;
        std::set<int> ids;
        auto query = mysqli_query(
            mysql,
            "SELECT id, title, description, owner, createdAt FROM teams,team_members WHERE teams.id = team_members.tid AND uid = %d AND (title LIKE \"%%%s%%\" OR description LIKE \"%%%s%%\")",
            uid,
            quote_encode(keyword).c_str(),
            quote_encode(keyword).c_str()
        );
        for (int i = 0; i < query.size(); i++) {
            if (ids.count(stoi(query[i]["id"]))) continue;
            ids.insert(stoi(query[i]["id"]));
            res.push_back(Team({
                .tid = stoi(query[i]["id"]),
                .title = query[i]["title"],
                .description = query[i]["description"],
                .owner = UserUtils.getUserInfo(stoi(query[i]["owner"])),
                .createdAt = stoll(query[i]["createdAt"]),
            }));
        }

        std::sort(res.begin(), res.end(), [&](Team a, Team b){
            switch (order) {
                case TeamSortOrder::NAME:
                    return a.title < b.title;
                case TeamSortOrder::CREATEDAT:
                    return a.createdAt < b.createdAt;
                case TeamSortOrder::DEFAULT:
                default:
                    return a.tid > b.tid;
            }
        });

        return res;
    }

    //编辑团队信息
    void edit(Team team) 
    {
        quick_mysqli_connect();
        mysqli_execute(
            mysql,
            "update teams set title=\"%s\",description=\"%s\" where id=%d",
            quote_encode(team.title).c_str(),
            quote_encode(team.description).c_str(),
            team.tid
        );
        mysqli_execute(mysql,
            "delete from team_members where tid=%d",
            team.tid
        );
        auto members = team.members;
        if(members.size() == 0)
        {
            members.push_back(User({
                .uid = team.owner.uid,
            }));
        }
        std::vector<std::string> v;
        for(size_t i = 0;i < members.size();++i)
        {
            v.push_back("(" + std::to_string(team.tid) + "," + std::to_string(members[i].uid) + ")");
        }
        std::string ins = "values" + join(",",v);
        mysqli_execute(
            mysql,
            "insert into team_members(tid,uid) %s",
             ins.c_str()
        );
    }
    
    void remove(int tid)
    {
        quick_mysqli_connect();
        mysqli_execute(
            mysql,
            "delete from teams where id = %d",
            tid
        );

        mysqli_execute(
            mysql,
            "delete from team_members where tid = %d",
            tid
        );
    }

    // 邀请团队成员
    void invite(int tid, std::vector<int> uids, std::string origin) {
        quick_mysqli_connect();

        std::vector<std::string> uids2 = {};
        for (int i = 0; i < uids.size(); i++) uids2.push_back(std::to_string(uids[i]));
        auto uids3 = mysqli_query(
            mysql,
            "SELECT id AS uid FROM users WHERE id IN (%s) "
            "EXCEPT "
            "SELECT uid FROM team_members WHERE tid = %d AND uid IN (%s)",
            join(", ", uids2).c_str(),
            tid,
            join(", ", uids2).c_str()
        );
        if (uids3.size() == 0) return;
        time_t createTime = time(NULL);
        time_t expireTime = createTime + appConfig["teams.inviteExpireTime"].asInt64() * 60 * 60;

        auto team = getTeamsInfo(tid);
        std::vector<std::string> codes;
        std::vector<std::string> team_invites;
        std::vector<std::string> msgs;
        std::vector<std::string> messages;
        std::vector<std::string> unread_marks;
        for (int i = 0; i < uids3.size(); i++) {
            std::string msg = invite_team_md;
            std::string code = generateSession();
            msg = str_replace("{{ users }}", team.owner.name, msg);
            msg = str_replace("{{ team }}", team.title, msg);
            msg = str_replace("{{ url }}", origin + "/invite?type=team&code=" + code, msg);

            msgs.push_back(msg);
            team_invites.push_back("(" + 
                std::to_string(tid) + ", " + 
                uids3[i]["uid"] + ", "
                "\"" + code + "\", " + 
                std::to_string(expireTime) + 
            ")");
            messages.push_back("("
                "\"system-invites\", " +
                uids3[i]["uid"] + ", "
                "\"" + quote_encode(msg) + "\", " +
                std::to_string(createTime) + 
            ")");
        }

        mysqli_execute(
            mysql,
            "INSERT INTO team_invites (tid, uid, code, expiredAt) VALUES %s",
            join(", ", team_invites).c_str()
        );
        mysqli_execute(
            mysql,
            "INSERT INTO messages (talkId, uid, message, createdAt) VALUES %s",
            join(", ", messages).c_str()
        );
        int id = stoi(mysqli_query(
            mysql,
            "SELECT MAX(mid) AS id FROM messages WHERE talkId = \"%s\" AND uid = %d",
            "system-invites",
            stoi(uids3[0]["uid"])
        )[0]["id"]);
        for (int i = 0; i < uids3.size(); i++) {
            unread_marks.push_back("(" +
                std::to_string(id + i) + ", " +
                uids3[i]["uid"] +
            ")");
        }
        mysqli_execute(
            mysql,
            "INSERT INTO unread_marks (mid, uid) VALUES %s",
            join(", ", unread_marks).c_str()
        );

        for (int i = 0; i < uids3.size(); i++) {
            int touid = stoi(uids3[i]["uid"]);
            Message msg = Message({
                .mid = id + i,
                .user = {},
                .message = msgs[i],
                .createdAt = createTime
            });

            Connection conn = Client("/tmp/ascas/msgUnread.sock").connect();
            conn.send("send");
            conn.send(std::to_string(touid));
            conn.send("1");
            int cnt = stoi(conn.recv());
            conn.close();

            conn = Client("/tmp/ascas/msgList.sock").connect();
            conn.send("send");
            conn.send(std::to_string(touid));
            conn.send("system-invites\r\n" + json_encode(Json::Value(msg)));
            cnt = stoi(conn.recv());
            conn.close();

            conn = Client("/tmp/ascas/msgDetails.sock").connect();
            conn.send("send");
            conn.send(std::to_string(touid));
            conn.send("system-invites\r\n" + json_encode(Json::Value(msg)));
            cnt = stoi(conn.recv());
            conn.close();
        }
    }
}TeamUtils;