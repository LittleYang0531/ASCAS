#pragma once
#include "../ascas.h"

class CropUtils {
    private:

    const std::string sessionStrings = 
        "0123456789abcdef";
    std::string generateSession(int len = 64) {
        srand(clock3());
        std::string session = "";
        for (int i = 0; i < len; i++) session += sessionStrings[rand() % sessionStrings.size()];
        return session;
    }

    std::string getPropertyType(RecordPropertyType type) {
        switch (type) {
            case RecordPropertyType::NUMBER: return " float"; break;
            case RecordPropertyType::DATE: return " int"; break;
            default: return " text"; break;
        }
    }

    public:

    // 检查作物是否存在
    int exists(int cid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM crops WHERE id = %d",
            cid
        )[0]["count"]);
    }

    // 获取作物信息
    Crop getCropInfo(int cid, int uid) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT * FROM crops WHERE id = %d",
            cid
        );
        MyAssert(
            res.size(),
            "cid %d not found, please use CropUtils.exists(%d) to check whether cid %d is exists before calling this method.",
            cid, cid, cid
        );
        auto crop = res[0];

        Crop item = Crop({
            .cid = stoi(crop["id"]),
            .name = crop["name"],
            .title = crop["title"],
            .description = crop["description"],
            .properties = extarr<RecordProperty>(json_decode(crop["properties"])),
            .owner = UserUtils.getUserInfo(stoi(crop["owner"])),
            .editors = extarr<User>(
                json_decode(crop["editors"]), 
                [](Json::Value val){ return User({ .uid = val.asInt() }); }
            ),
            .viewers = extarr<User>(
                json_decode(crop["viewers"]), 
                [](Json::Value val){ return User({ .uid = val.asInt() }); }
            ),
            .createdAt = stoll(crop["createdAt"]),
            .updatedAt = stoll(crop["updatedAt"])
        });
        
        if (uid == item.owner.uid) item.permission = UserPermission::OWNER;
        else {
            for (int i = 0; i < item.viewers.size(); i++)
                if (uid == item.viewers[i].uid) item.permission = UserPermission::VIEWER;
            for (int i = 0; i < item.editors.size(); i++)
                if (uid == item.editors[i].uid) item.permission = UserPermission::EDITOR;
        }

        if (item.permission == UserPermission::NONE) item = Crop({ .permission = UserPermission::NONE });
        else {
            for (int i = 0; i < item.viewers.size(); i++) item.viewers[i] = UserUtils.getUserInfo(item.viewers[i].uid);
            for (int i = 0; i < item.editors.size(); i++) item.editors[i] = UserUtils.getUserInfo(item.editors[i].uid);
        }

        return item;
    }

    // 创建作物表
    int create(Crop crop) {
        quick_mysqli_connect();
        crop.name = generateSession(16);
        for (int i = 0; i < crop.properties.size(); i++)
            crop.properties[i].name = generateSession(16);
        std::string properties = json_encode(packarr(crop.properties));
        std::string editors = json_encode(packarr(crop.editors, [](User u){ return u.uid; }));
        std::string viewers = json_encode(packarr(crop.viewers, [](User u){ return u.uid; }));
        mysqli_execute(
            mysql,
            "INSERT INTO crops (name, title, description, properties, owner, editors, viewers, createdAt, updatedAt) "
            "VALUES (\"%s\", \"%s\", \"%s\", \"%s\", %d, \"%s\", \"%s\", %lld, %lld)",
            crop.name.c_str(),
            quote_encode(crop.title).c_str(),
            quote_encode(crop.description).c_str(),
            quote_encode(properties).c_str(),
            crop.owner.uid,
            quote_encode(editors).c_str(),
            quote_encode(viewers).c_str(),
            time(NULL),
            time(NULL)
        );

        std::vector<std::string> columns;
        std::string type;
        for (int i = 0; i < crop.properties.size(); i++){
            type = getPropertyType(crop.properties[i].type);
            columns.push_back("var_" + crop.properties[i].name + type);
        }
        std::string columnString = join(", ", columns);
        mysqli_execute(
            mysql,
            "CREATE TABLE table_%s (id int AUTO_INCREMENT PRIMARY KEY, name text, uid int, %s)",
            crop.name.c_str(),
            columnString.c_str()
        );

        return stoi(mysqli_query(
            mysql,
            "SELECT id FROM crops WHERE name = \"%s\"",
            crop.name.c_str()
        )[0]["id"]);
    }

    // 列举相关作物表信息
    std::vector<Crop> listCrops(int uid, std::string keyword, UserPermission perm, CropSortOrder order) {
        quick_mysqli_connect();
        std::vector<Crop> res;
        std::set<int> ids;
        
        if (perm == UserPermission::NONE || perm == UserPermission::OWNER) {
            auto query = mysqli_query(
                mysql,
                "SELECT id, name, title, description, owner, createdAt, updatedAt FROM crops WHERE owner = %d AND (title LIKE \"%%%s%%\" OR description LIKE \"%%%s%%\")",
                uid,
                quote_encode(keyword).c_str(),
                quote_encode(keyword).c_str()
            );
            for (int i = 0; i < query.size(); i++) {
                if (ids.count(stoi(query[i]["id"]))) continue;
                ids.insert(stoi(query[i]["id"]));
                res.push_back(Crop({
                    .cid = stoi(query[i]["id"]),
                    .name = query[i]["name"],
                    .title = query[i]["title"],
                    .description = query[i]["description"],
                    .owner = UserUtils.getUserInfo(stoi(query[i]["owner"])),
                    .createdAt = stoll(query[i]["createdAt"]),
                    .updatedAt = stoll(query[i]["updatedAt"]),
                    .permission = UserPermission::OWNER
                }));
            }
        }
        
        if (perm == UserPermission::NONE || perm == UserPermission::EDITOR) {
            auto query = mysqli_query(
                mysql,
                "SELECT id, name, title, description, owner, createdAt, updatedAt FROM crops WHERE %s AND (title LIKE \"%%%s%%\" OR description LIKE \"%%%s%%\")",
                hasIntersection("editors", jsonarr(uid)).c_str(),
                quote_encode(keyword).c_str(),
                quote_encode(keyword).c_str()
            );
            for (int i = 0; i < query.size(); i++) {
                if (ids.count(stoi(query[i]["id"]))) continue;
                ids.insert(stoi(query[i]["id"]));
                res.push_back(Crop({
                    .cid = stoi(query[i]["id"]),
                    .name = query[i]["name"],
                    .title = query[i]["title"],
                    .description = query[i]["description"],
                    .owner = UserUtils.getUserInfo(stoi(query[i]["owner"])),
                    .createdAt = stoll(query[i]["createdAt"]),
                    .updatedAt = stoll(query[i]["updatedAt"]),
                    .permission = UserPermission::EDITOR
                }));
            }
        }
        
        if (perm == UserPermission::NONE || perm == UserPermission::VIEWER) {
            auto query = mysqli_query(
                mysql,
                "SELECT id, name, title, description, owner, createdAt, updatedAt FROM crops WHERE %s AND (title LIKE \"%%%s%%\" OR description LIKE \"%%%s%%\")",
                hasIntersection("viewers", jsonarr(uid)).c_str(),
                quote_encode(keyword).c_str(),
                quote_encode(keyword).c_str()
            );
            for (int i = 0; i < query.size(); i++) {
                if (ids.count(stoi(query[i]["id"]))) continue;
                ids.insert(stoi(query[i]["id"]));
                res.push_back(Crop({
                    .cid = stoi(query[i]["id"]),
                    .name = query[i]["name"],
                    .title = query[i]["title"],
                    .description = query[i]["description"],
                    .owner = UserUtils.getUserInfo(stoi(query[i]["owner"])),
                    .createdAt = stoll(query[i]["createdAt"]),
                    .updatedAt = stoll(query[i]["updatedAt"]),
                    .permission = UserPermission::VIEWER
                }));
            }
        }

        std::sort(res.begin(), res.end(), [&](Crop a, Crop b){
            switch (order) {
                case CropSortOrder::NAME:
                    return a.title < b.title;
                case CropSortOrder::CREATEDAT:
                    return a.createdAt < b.createdAt;
                case CropSortOrder::UPDATEDAT:
                    return a.updatedAt < b.updatedAt;
                case CropSortOrder::DEFAULT:
                default:
                    return a.permission == b.permission ? a.cid > b.cid : a.permission > b.permission;
            }
        });

        return res;
    }

    //编辑作物属性
    void edit(Crop previous_crop,Crop current_crop) {
        quick_mysqli_connect();
        std::vector<std::string> add;
        for(int i = 0;i < current_crop.properties.size();++i)
        {
            bool a = false;
            for(int j = 0;j < previous_crop.properties.size();++j)
            {
                if(current_crop.properties[i].name == previous_crop.properties[j].name) {
                
                    a = true;
                    break;
                }
            }
            if(!a) {
                current_crop.properties[i].name = generateSession(16);
                add.push_back(current_crop.properties[i].name);
            }
        }

        std::vector<std::string> del;
        for(int i = 0;i < previous_crop.properties.size();++i)
        {
            bool a = false;
            for(int j = 0;j < current_crop.properties.size();++j)
            {
                if(current_crop.properties[j].name == previous_crop.properties[i].name){
                    a = true;
                    break;
                }
            }
            if(!a){
                del.push_back(previous_crop.properties[i].name);
            }
        }

        std::string all,sep = ",";
        std::vector<std::string> v;

        for(size_t i = 0;i < del.size();++i) 
        { 
            std::string v1 = " drop var_" + del[i];
            v.push_back(v1);
        }
        for(size_t i = 0;i < add.size();++i) 
        { 
            std::string v1 = " add var_" + add[i];
            std::string type = getPropertyType(current_crop.properties[i].type);
            v1 += type;
            v.push_back(v1);
        }  

        all = join(sep,v);

        std::string properties = json_encode(packarr(current_crop.properties));
        std::string editors = json_encode(packarr(current_crop.editors, [](User u){ return u.uid; }));
        std::string viewers = json_encode(packarr(current_crop.viewers, [](User u){ return u.uid; }));
        mysqli_execute(mysql,
            "update crops set title=\"%s\",description=\"%s\",viewers=\"%s\",editors=\"%s\",properties=\"%s\" "
            "where id= %d",
            quote_encode(current_crop.title).c_str(),
            quote_encode(current_crop.description).c_str(),
            quote_encode(viewers).c_str(),
            quote_encode(editors).c_str(),
            quote_encode(properties).c_str(),
            previous_crop.cid
        );

        mysqli_execute(mysql,
            "alter table table_%s %s",
            quote_encode(previous_crop.name).c_str(),
            quote_encode(all).c_str()
        );

        //修改顺序的逻辑
        std::vector<std::string> astr;
        for(int i = 1;i < current_crop.properties.size();++i)
        {
            std::string type = getPropertyType(current_crop.properties[i].type);
            astr.push_back("modify var_" + current_crop.properties[i].name + type + " after var_" + current_crop.properties[i - 1].name);
        }
        std::string l = join(sep,astr);
        std::string type1 = getPropertyType(current_crop.properties[0].type);
        mysqli_execute(mysql,
            "alter table table_%s "
            "MODIFY id int AUTO_INCREMENT FIRST, "
            "MODIFY name text AFTER id, "
            "MODIFY uid int AFTER name, "
            "modify var_%s %s after uid,"
            "%s",
            quote_encode(previous_crop.name).c_str(),
            quote_encode(current_crop.properties[0].name).c_str(),
            type1.c_str(),
            quote_encode(l).c_str()
        );

    }

    void remove(Crop crop)
    {
        //delete from crops where name
        quick_mysqli_connect();
        std::string cropname = crop.name;
        mysqli_execute(
            mysql,
            "drop table table_%s",
            quote_encode(cropname).c_str()
        );
        std::string sep = "\"";
        mysqli_execute(
            mysql,
            "delete from crops where name = %s",
            (sep + cropname + sep).c_str()
        );
    }

    void invite(int cid, int uid, std::vector<int> uids, bool editor, std::string origin) {
        quick_mysqli_connect();

        Json::Value users = json_decode(mysqli_query(
            mysql,
            "SELECT %s FROM crops WHERE id = %d",
            editor ? "editors" : "viewers",
            cid
        )[0][editor ? "editors" : "viewers"]);
        std::vector<int> uids2 = {};
        for (int i = 0; i < uids.size(); i++) {
            bool exists = false;
            for (int j = 0; j < users.size(); j++) exists |= uids[i] == users[j].asInt();
            if (!exists) uids2.push_back(uids[i]);
        }
        if (uids2.size() == 0) return;
        time_t createTime = time(NULL);
        time_t expireTime = createTime + appConfig["teams.inviteExpireTime"].asInt64() * 60 * 60;

        auto crop = getCropInfo(cid, uid);
        std::vector<std::string> codes;
        std::vector<std::string> crop_invites;
        std::vector<std::string> msgs;
        std::vector<std::string> messages;
        std::vector<std::string> unread_marks;
        for (int i = 0; i < uids2.size(); i++) {
            std::string msg = invite_crop_md;
            std::string code = generateSession();
            msg = str_replace("{{ users }}", crop.owner.name, msg);
            msg = str_replace("{{ perm }}", editor ? "编辑" : "查看", msg);
            msg = str_replace("{{ crop }}", crop.title, msg);
            msg = str_replace("{{ url }}", origin + "/invite?type=crop&cid=" + std::to_string(cid) + "&code=" + code, msg);

            msgs.push_back(msg);
            crop_invites.push_back("(" + 
                std::to_string(cid) + ", " + 
                std::to_string(uids2[i]) + ", "
                "\"" + code + "\", " + 
                std::string(editor ? "TRUE" : "FALSE") + ", " +
                std::to_string(expireTime) + 
            ")");
            messages.push_back("("
                "\"system-invites\", " +
                std::to_string(uids2[i]) + ", "
                "\"" + quote_encode(msg) + "\", " +
                std::to_string(createTime) + 
            ")");
        }

        mysqli_execute(
            mysql,
            "INSERT INTO crop_invites (cid, uid, code, isEditor, expiredAt) VALUES %s",
            join(", ", crop_invites).c_str()
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
            uids2[0]
        )[0]["id"]);
        for (int i = 0; i < uids2.size(); i++) {
            unread_marks.push_back("(" +
                std::to_string(id + i) + ", " +
                std::to_string(uids2[i]) +
            ")");
        }
        mysqli_execute(
            mysql,
            "INSERT INTO unread_marks (mid, uid) VALUES %s",
            join(", ", unread_marks).c_str()
        );

        for (int i = 0; i < uids2.size(); i++) {
            int touid = uids2[i];
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

    bool checkInvite(int cid, int uid, std::string code) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM crop_invites WHERE cid = %d AND uid = %d AND code = \"%s\" AND expiredAt >= %lld",
            cid,
            uid,
            code.c_str(),
            time(NULL)
        )[0]["count"]);
    }

    void acceptInvite(int cid, int uid, std::string code) {
        quick_mysqli_connect();

        auto invite = mysqli_query(
            mysql,
            "SELECT * FROM crop_invites WHERE cid = %d AND uid = %d AND code = \"%s\" AND expiredAt >= %lld",
            cid,
            uid,
            code.c_str(),
            time(NULL)
        )[0];
        auto crop = mysqli_query(
            mysql,
            "SELECT viewers, editors FROM crops WHERE id = %d",
            cid
        )[0];

        bool isEditor = stoi(invite["isEditor"]);
        Json::Value viewers = json_decode(crop["viewers"]);
        Json::Value editors = json_decode(crop["editors"]);
        bool exists = false;
        for (int i = 0; i < viewers.size(); i++) exists |= viewers[i].asInt() == uid;
        if (!exists) viewers.append(uid);
        exists = false;
        for (int i = 0; i < editors.size(); i++) exists |= editors[i].asInt() == uid;
        if (!exists && isEditor) editors.append(uid);
        mysqli_execute(
            mysql,
            "UPDATE crops SET viewers = \"%s\", editors = \"%s\" WHERE id = %d",
            json_encode(viewers).c_str(),
            json_encode(editors).c_str(),
            cid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM crop_invites WHERE cid = %d AND uid = %d AND code = \"%s\"",
            cid,
            uid,
            code.c_str()
        );
    }
}CropUtils;