#pragma once
#include "../ascas.h"

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

public:
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
                newline += ((posts[crop.properties[i].name].asString()));
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
            all.c_str());
    }
} RecordUtils;