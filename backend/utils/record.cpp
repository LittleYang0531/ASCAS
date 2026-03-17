#pragma once
#include "../ascas.h"
#include<queue>
class RecordUtils
{
private:
    const std::string sessionStrings =
        "0123456789abcdef";
    std::string generateSession(int len = 64)
    {
        srand(clock3());
        std::string session = "";
        for (int i = 0; i < len; i++)
            session += sessionStrings[rand() % sessionStrings.size()];
        return session;
    }
    int pos(const std::vector<RecordProperty>& v,std::string s)
    {
        for(size_t i = 0;i < v.size();++i)
        {
            if(v[i].name == s)
            {
                return i;
            }
        }
        return -1;
    }
    bool isLegal(std::string type,SQLOperator op)
    {
        if(type == "FLOAT") {
            if(op == SQLOperator::LIKE || op == SQLOperator::NOTLIKE || 
            op == SQLOperator::REGEXP || op == SQLOperator::NOTREGEXP) return false;
        }
        else {
            if(op == SQLOperator::GREATER || op == SQLOperator::GREATER_OR_EQUAL || 
            op == SQLOperator::SMALLER || op == SQLOperator::SMALLER_OR_EQUAL) return false;
        }
        return true;
    }
    SQLOperator getOp(Json::Value posts)
    {
        return getEnumFromName(SQLOperator,posts["op"].asString());
    }
    std::string getColumnType(Json::Value posts)
    {
        return posts["column"].asString();
    }
    std::string getCropType(const std::vector<RecordProperty>& vrecord,size_t posi)
    {
        return (vrecord[posi].type == RecordPropertyType::NUMBER ? "FLOAT" : "TEXT");
    }
    bool isLeaf(Json::Value posts)
    {
        return posts["isLeaf"].asBool();
    }
    std::string opToSql(Json::Value posts,std::string type)
    {
        SQLOperator op = getOp(posts);
        if(!isLegal(type,op)) return "";
        if(op == SQLOperator::EQUAL) return "= ";
        else if(op == SQLOperator::NOTEQUAL) return "!= ";
        else if(op == SQLOperator::GREATER) return "> ";
        else if(op == SQLOperator::GREATER_OR_EQUAL) return ">= ";
        else if(op == SQLOperator::SMALLER) return "< ";
        else if(op == SQLOperator::SMALLER_OR_EQUAL)   return "<= ";
        else if(op == SQLOperator::LIKE) return "like ";
        else if(op == SQLOperator::NOTLIKE) return "not like ";
        else if(op == SQLOperator::REGEXP) return "regexp ";
        else return "not regexp ";
    }
    std::string getValue(Json::Value posts)
    {
        return posts["value"].asString();
    }
    std::string getTypeName(const std::vector<RecordProperty>& v,int pos) //pos已经判断是否合法了
    {
        return v[pos].name + " ";
    }
    bool isAnd(Json::Value posts)
    {
        return posts["isAnd"].asBool();
    }
    std::string getWhereLine(Json::Value posts,const std::vector<RecordProperty>& vrecord)
    {
        std::string ctype = getColumnType(posts);
        int posi = pos(vrecord,getColumnType(posts));
        if(posi == -1) return ""; // id uid
        std::string type = getCropType(vrecord,posi),op = opToSql(posts,type);
        if(op.size() == 0) return "";
        std::string value = getValue(posts);
        if(type == "TEXT") value = "\"" + value + "\"";
        value += ")";
        return ("(var_" + getTypeName(vrecord,posi) + op + value);
    }
    std::string getOrder(Json::Value posts)
    {
        return (posts["isASC"].asBool() == true ?  "asc" : "desc");
    }
public:
    int list(Crop crop,Json::Value posts)
    {
        std::string whereall,orderall,limitall;
        std::vector<RecordProperty> vrecord = crop.properties;
        if(posts.isMember("where"))
        {
        std::vector<bool> connectv;//记录下一条语句的连接关系的数组,最后一起拼接
        std::vector<std::string> wherev; //记录语句，最后拼接
        std::queue<Json::Value> whereq; //遍历树的队列
            whereq.push(posts["where"]);
            while(whereq.size())
            {
                auto p = whereq.front();
                whereq.pop();
                if(!isLeaf(p))
                {
                    connectv.push_back(isAnd(p));

                    for(int i = 0;i < p["params"].size();++i)
                    {
                        whereq.push(p["params"][i]);
                    }
                }
                else 
                {
                    std::string ret = getWhereLine(p,vrecord);
                    if(ret.size() == 0) return -1;
                    wherev.push_back(ret);
                }
            }
            for(int i = 0;i < wherev.size();++i)
            {
                whereall += wherev[i];
                
                if(i != wherev.size() - 1) {
                    whereall += (connectv[i] == true ? "and ": "or ");
                }
            }
            std::cout << whereall << std::endl;
        }
        if(posts.isMember("order"))
        {
            std::vector<std::string> v;
           for(int i = 0;i < posts["order"].size();++i)
           {
               std::string type = getColumnType(posts["order"][i]);
               if(type == "id") {}
               else if(type == "uid") {}
               else if(type == "name") {}
               else
               {
               int ret = pos(vrecord,type);
               if(ret == -1) return -1;
               }
               type = "var_" + type;
               std::string order = getOrder(posts["order"][i]);
               v.push_back(type + " " + order);
           }
           orderall = "order by " + join(",",v);
        }

        if(posts.isMember("limit")) {
            int a = posts["limit"].asInt(), b = posts["offset"].asInt();
            limitall = "limit " + std::to_string(a) +  " offset " + std::to_string(b);
        }

        quick_mysqli_connect();
        std::string all = whereall + " " + orderall + " " + limitall;
        mysqli_execute(mysql,"select * from table_%s where "
            "%s",
            quote_encode(crop.name).c_str(),
            (all).c_str()
        );
        
        return 0;
    }

    void add(Crop crop, Json::Value posts, int uid)
    {

        std::string all, sep = ",", c = "\"",var;
        size_t n = crop.properties.size();
        std::vector<std::string> vjoin,k;
        for (int i = 0; i < n; ++i)
        {
            std::string newline;
            if (crop.properties[i].type == RecordPropertyType::NUMBER)
            {
                newline += std::to_string(stod((posts[crop.properties[i].name].asString())));
            }
            else
            {
                newline += c;
                newline += quote_encode(posts[crop.properties[i].name].asString());
                newline += c;
            }
            k.push_back("var_" + crop.properties[i].name);
            vjoin.push_back(newline);
        }
        all = join(sep, vjoin);
        var = join(sep, k);
        std::string name = c + generateSession(16) + c;
        quick_mysqli_connect();
        mysqli_execute(
            mysql,
            "insert into table_%s(name,uid,%s) values(%s,%d,%s)",
            quote_encode(crop.name).c_str(),
            quote_encode(var).c_str(),
            name.c_str(),
            uid,
            all.c_str()
        );
    }
}RecordUtils;