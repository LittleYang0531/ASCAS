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
        if (item.permission != UserPermission::OWNER) item.viewers.clear(), item.editors.clear();

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
        for (int i = 0; i < crop.properties.size(); i++)
            columns.push_back("var_" + crop.properties[i].name + " text");
        std::string columnString = join(", ", columns);
        mysqli_execute(
            mysql,
            "CREATE TABLE table_%s (%s)",
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
        
        if (perm == UserPermission::NONE || perm == UserPermission::OWNER) {
            auto query = mysqli_query(
                mysql,
                "SELECT id, name, title, description, owner, createdAt, updatedAt FROM crops WHERE owner = %d AND (title LIKE \"%%%s%%\" OR description LIKE \"%%%s%%\")",
                uid,
                quote_encode(keyword).c_str(),
                quote_encode(keyword).c_str()
            );
            for (int i = 0; i < query.size(); i++) {
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
                    return a.permission == b.permission ? a.cid < b.cid : a.permission > b.permission;
            }
        });

        return res;
    }
}CropUtils;