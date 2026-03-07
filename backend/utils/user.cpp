#pragma once
#include "../ascas.h"

class UserUtils {
    private:

    const std::string authorizationPrefix = "SessionToken";
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

    void clearSessions() {
        quick_mysqli_connect();
        time_t t = time(NULL);
        mysqli_execute(
            mysql,
            "DELETE FROM sessions WHERE expireAt < %lld", 
            t
        );
        mysqli_execute(
            mysql,
            "DELETE FROM verifications WHERE expireAt < %lld",
            t
        );
        mysqli_execute(
            mysql,
            "DELETE FROM resetCodes WHERE expireAt < %lld",
            t
        );
        mysqli_execute(
            mysql,
            "DELETE FROM users WHERE verified = false AND id not in (SELECT uid FROM verifications)",
            t
        );
    }

    public:

    // 检查登录态
    int checkLogin(http_request request) {
        quick_mysqli_connect();
        std::string session = "";
        auto cookies = cookieParam(request);
        if (cookies.find("session") != cookies.end()) session = cookies["session"];
        if (request.argv.find("authorization") != request.argv.end()) {
            std::string authorization = request.argv["authorization"];
            if (authorization.substr(0, authorizationPrefix.size()) == authorizationPrefix)
                session = authorization.substr(authorizationPrefix.size() + 1);
        }
        clearSessions();
        time_t t = time(NULL);
        auto res = mysqli_query(
            mysql,
            "SELECT uid FROM sessions WHERE session = \"%s\" AND expireAt >= %lld",
            session.c_str(),
            t
        );
        return res.size() != 1 ? 0 : stoi(res[0]["uid"]);
    }

    // 检查用户是否存在
    int exists(int uid) {
        quick_mysqli_connect();
        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM users WHERE id = %d",
            uid
        )[0]["count"]);
    }

    // 获取用户信息
    User getUserInfo(int uid) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT * FROM users WHERE id = %d",
            uid
        );
        MyAssert(
            res.size(),
            "uid %d not found, please use UserUtils.exists(%d) to check whether uid %d is exists before calling this method.",
            uid, uid, uid
        );
        auto user = res[0];
        return User({
            .uid = stoi(user["id"]),
            .name = user["name"],
            .email = user["email"],
            .isAdmin = stoi(user["isAdmin"]) ? true : false,
        });
    }

    // 检查用户名是否被占用
    int checkName(std::string name) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT id FROM users WHERE name = \"%s\"",
            quote_encode(name).c_str()
        );
        return res.size() == 0 ? 0 : stoi(res[0]["id"]);
    }

    // 检查用户邮箱是否被占用
    int checkEmail(std::string email) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT id FROM users WHERE email = \"%s\"",
            quote_encode(email).c_str()
        );
        return res.size() == 0 ? 0 : stoi(res[0]["id"]);
    }

    // 用户登录
    std::string login(std::string name, std::string passwd) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT id, verified FROM users WHERE (name = \"%s\" OR email = \"%s\") AND passwd = \"%s\"",
            quote_encode(name).c_str(),
            quote_encode(name).c_str(),
            quote_encode(passwd).c_str()
        );
        if (res.size() == 0) return "";
        int verified = stoi(res[0]["verified"]);
        if (!verified) return "not_verified";
        int uid = stoi(res[0]["id"]);
        std::string session = generateSession();
        mysqli_execute(
            mysql,
            "INSERT INTO sessions VALUES (%d, \"%s\", %lld)",
            uid,
            session.c_str(),
            time(NULL) + appConfig["users.sessionExpireTime"].asInt64() * 24 * 60 * 60  
        );
        return session;
    }

    // 创建用户
    void create(User info, std::string passwd, std::string origin) {
        quick_mysqli_connect();
        mysqli_execute(
            mysql,
            "INSERT INTO users (name, passwd, email, verified, isAdmin) VALUES (\"%s\", \"%s\", \"%s\", false, %d)",
            quote_encode(info.name).c_str(),
            quote_encode(passwd).c_str(),
            quote_encode(info.email).c_str(),
            info.isAdmin
        );
        int uid = stoi(mysqli_query(
            mysql,
            "SELECT id FROM users WHERE name = \"%s\" AND email = \"%s\"",
            quote_encode(info.name).c_str(),
            quote_encode(info.email).c_str()
        )[0]["id"]);

        std::string code = generateSession(128);
        mysqli_execute(
            mysql,
            "INSERT INTO verifications (uid, code, expireAt) VALUES (%d, \"%s\", %lld)",
            uid,
            code.c_str(),
            time(NULL) + appConfig["users.verifyExpireTime"].asInt64() * 60 * 60
        );

        std::string data = signup_html;
        data = str_replace("{{ link }}", origin + "/login?verify&code=" + code, data);
        data = str_replace("{{ name }}", info.name, data);
        SMTP smtp(
            appConfig["smtp.host"].asString(),
            appConfig["smtp.port"].asInt(),
            appConfig["smtp.ssl"].asBool(),
            appConfig["smtp.tls"].asBool()
        );
        smtp.login(appConfig["smtp.user"].asString(), appConfig["smtp.passwd"].asString());
        smtp.rcpt(info.email);
        smtp.mail("邮箱验证", data);
    }

    // 邮箱验证
    bool verify(std::string code) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT * FROM verifications WHERE code = \"%s\" AND expireAt >= %lld",
            code.c_str(),
            time(NULL)
        );
        if (res.size() == 0) return false;

        int uid = stoi(res[0]["uid"]);
        mysqli_execute(
            mysql,
            "UPDATE users SET verified = true WHERE id = %d",
            uid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM verifications WHERE code = \"%s\"",
            code.c_str()
        );
        return true;
    }

    // 创建重置密码请求
    bool reset_request(std::string email, std::string origin) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT id, name FROM users WHERE email = \"%s\"",
            email.c_str()
        );
        if (res.size() == 0) return false;
        int uid = stoi(res[0]["id"]);
        std::string name = res[0]["name"];

        std::string code = generateSession(128);
        mysqli_execute(
            mysql,
            "INSERT INTO resetCodes (uid, code, expireAt) VALUES (%d, \"%s\", %lld)",
            uid,
            code.c_str(),
            time(NULL) + appConfig["users.resetExpireTime"].asInt64() * 60 * 60
        );

        std::string data = reset_html;
        data = str_replace("{{ link }}", origin + "/login?reset_verify&code=" + code, data);
        data = str_replace("{{ name }}", name, data);
        SMTP smtp(
            appConfig["smtp.host"].asString(),
            appConfig["smtp.port"].asInt(),
            appConfig["smtp.ssl"].asBool(),
            appConfig["smtp.tls"].asBool()
        );
        smtp.login(appConfig["smtp.user"].asString(), appConfig["smtp.passwd"].asString());
        smtp.rcpt(email);
        smtp.mail("密码重置", data + "\r\n\r\n");
        return true;
    }

    // 验证请求密钥
    int reset_verify(std::string code) {
        quick_mysqli_connect();
        auto res = mysqli_query(
            mysql,
            "SELECT uid FROM resetCodes WHERE code = \"%s\" AND expireAt >= %lld",
            code.c_str(),
            time(NULL)
        );
        return res.size() == 0 ? 0 : stoi(res[0]["uid"]);
    }

    // 重置密码
    bool reset(std::string code, std::string passwd) {
        quick_mysqli_connect();
        int uid = reset_verify(code);
        if (uid == 0) return false;
        
        mysqli_execute(
            mysql,
            "UPDATE users SET passwd = \"%s\" WHERE id = %d",
            quote_encode(passwd).c_str(),
            uid
        );
        mysqli_execute(
            mysql,
            "DELETE FROM resetCodes WHERE code = \"%s\"",
            code.c_str()
        );
        return true;
    }

    std::vector<User> search(std::string keyword) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT * FROM users WHERE name LIKE \"%%%s%%\" OR email LIKE \"%%%s%%\"",
            quote_encode(keyword).c_str(),
            quote_encode(keyword).c_str()
        );

        std::vector<User> results;
        for (int i = 0; i < res.size(); i++) {
            results.push_back(User({
                .uid = stoi(res[i]["id"]),
                .name = res[i]["name"],
                .email = res[i]["email"],
                .isAdmin = stoi(res[i]["isAdmin"]) ? true : false
            }));
        }

        return results;
    }
}UserUtils;