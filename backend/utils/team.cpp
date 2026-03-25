#pragma once
#include "../ascas.h"

class TeamUtils {
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

}TeamUtils;