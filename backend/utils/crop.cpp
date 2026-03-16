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
            type = (crop.properties[i].type == RecordPropertyType::NUMBER ? " float": " text");
            columns.push_back("var_" + crop.properties[i].name + type);
        }
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
            std::string type = (current_crop.properties[i].type == RecordPropertyType::NUMBER ? " FLOAT" : " TEXT");
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
            std::string type = (current_crop.properties[i].type == RecordPropertyType::NUMBER ? " FLOAT" : " TEXT");
            astr.push_back("modify var_" + current_crop.properties[i].name + type + " after var_" + current_crop.properties[i - 1].name);
        }
        std::string l = join(sep,astr);
        if(l != "") l = ',' + l;
        std::string type1 = (current_crop.properties[0].type == RecordPropertyType::NUMBER ? " FLOAT" : " TEXT");
        mysqli_execute(mysql,
            "alter table table_%s modify var_%s %s first"
            "%s",
            quote_encode(previous_crop.name).c_str(),
            quote_encode(current_crop.properties[0].name).c_str(),
            quote_encode(type1).c_str(),
            quote_encode(l).c_str()
        );

    }
}CropUtils;