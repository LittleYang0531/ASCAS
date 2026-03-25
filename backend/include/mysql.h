#pragma once

#include "log.h"
#include <map>
#include <mysql/mysql.h>
#include <set>
#include <string>
#include <vector>

const std::set<int> retryedErrno = { 2013 };

class HintMap {
    public:

    bool enableHint = false;
    std::map<std::string, std::string> res;

    std::string& operator [] (std::string key) {
        if (enableHint && res.find(key) == res.end()) {
            std::string keyString = "";
            for (auto v : res) {
                if (keyString != "") keyString += ", ";
                keyString += v.first;
            }
            writeLog(
                LOG_LEVEL_WARNING, 
                "Unknown key \"%s\" in mysql query result, existed keys: (%s)",
                key.c_str(),
                keyString.c_str()
            );
        }

        return res[key];
    }
};

/**
 * @brief 连接数据库
 * @param host 数据库地址
 * @param user 数据库用户
 * @param passwd 数据库密码
 * @param db 数据库名
 * @param port 数据库端口
 * @return MySQL 连接符
 */
MYSQL mysqli_connect(std::string host, std::string user, std::string passwd, std::string db, int port) {
    MYSQL mysql, *res1; 
    res1 = mysql_init(&mysql); 
    if (res1 == NULL) writeLog(LOG_LEVEL_ERROR, "Failed to connect to database: %s(Errno: %d)", mysql_error(&mysql), mysql_errno(&mysql));
	bool res2 = mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0);
    if (!res2) {
        if (retryedErrno.count(mysql_errno(&mysql))) return mysqli_connect(host, user, passwd, db, port);
        else writeLog(LOG_LEVEL_ERROR, "Failed to connect to database: %s(Errno: %d)", mysql_error(&mysql), mysql_errno(&mysql));
    }
	return mysql;
}

/**
 * @brief 查询 SQL 语句
 * @param conn MySQL 连接符
 * @param sql SQL 语句
 * @return 查询结果
 */
std::vector<HintMap> mysqli_query(MYSQL &conn, std::string sql) {
    writeLog(LOG_LEVEL_DEBUG, "Querying SQL sentence: %s", sql.c_str());
    std::vector<HintMap> res; 
    bool res1 = mysql_query(&conn, sql.c_str());
    if (res1) {
        if (retryedErrno.count(mysql_errno(&conn))) return mysqli_query(conn, sql);
        else writeLog(LOG_LEVEL_ERROR, "Failed to query SQL sentence: %s(Errno: %d, SQL: \"%s\")", mysql_error(&conn), mysql_errno(&conn), sql.c_str());
    }
    MYSQL_RES *res2 = mysql_store_result(&conn);
    if (!res2) writeLog(LOG_LEVEL_ERROR, "Failed to query SQL sentence: %s(Errno: %d, SQL: \"%s\")", mysql_error(&conn), mysql_errno(&conn), sql.c_str());
	std::vector<std::string> field; MYSQL_FIELD *fd; MYSQL_ROW row;
    while((fd = mysql_fetch_field(res2))) field.push_back(fd->name);
	while ((row = mysql_fetch_row(res2))) {
        HintMap tmp;
		for (int i = 0; i < field.size(); i++) tmp[field[i]] = row[i] == NULL ? "" : row[i];
        tmp.enableHint = true;
		res.push_back(tmp);
	} mysql_free_result(res2);
	return res;
}

/**
 * @brief 查询 SQL 语句
 * @param conn MySQL 连接符
 * @param format SQL 格式
 * @return 查询结果
 */
std::vector<HintMap> mysqli_query(MYSQL &conn, const char* format, ...) {
    // 格式化文本
    const int size = 2 * 1024 * 1024;
    va_list args;
    va_start(args, format);
    char buf[size];
    memset(buf, '\0', sizeof buf);
    int n = vsnprintf(buf, size, format, args);
    va_end(args);
    if (n >= size) writeLog(LOG_LEVEL_WARNING, "Buffer size may be not enough! Real size: %d", n);
    return mysqli_query(conn, std::string(buf));
}

/**
 * @brief 执行 SQL 语句
 * @param conn MySQL 连接符
 * @param sql SQL 语句
 */
void mysqli_execute(MYSQL &conn, std::string sql) {
    writeLog(LOG_LEVEL_DEBUG, "Executing SQL sentence: %s", sql.c_str());
	if (mysql_query(&conn, sql.c_str())) {
        if (retryedErrno.count(mysql_errno(&conn))) mysqli_execute(conn, sql);
        else writeLog(LOG_LEVEL_ERROR, "Failed to execute SQL sentence: %s(Errno: %d, SQL: \"%s\")", mysql_error(&conn), mysql_errno(&conn), sql.c_str());
    }
}

/**
 * @brief 执行 SQL 语句
 * @param conn MySQL 连接符
 * @param format SQL 格式
 */
void mysqli_execute(MYSQL &conn, const char* format, ...) {
    // 格式化文本
    const int size = 2 * 1024 * 1024;
    va_list args;
    va_start(args, format);
    char buf[size];
    memset(buf, '\0', sizeof buf);
    int n = vsnprintf(buf, size, format, args);
    va_end(args);
    if (n >= size) writeLog(LOG_LEVEL_WARNING, "Buffer size may be not enough! Real size: %d", n);
    mysqli_execute(conn, std::string(buf));
}

void mysqli_close(MYSQL &conn) {
    mysql_close(&conn);
}

class DBColumn {
    public:

    std::string name;
    std::string type;
    bool isNull = true;
    bool enableDefaultValue = true;
    std::string defaultValue = "";
    bool valueIsExpr = false;
    bool isVisible = true;
    bool autoIncrement = false;
    bool unique = false;
    bool primary = false;
    std::string extraDef = "";

    std::string toString() {
        std::string str = "";
        str += name + " ";
        str += type + " ";
        if (!isNull) str += "NOT NULL ";
        if (enableDefaultValue) {
            if (!valueIsExpr) str += "DEFAULT (\"" + defaultValue + "\") ";
            else str += "DEFAULT " + defaultValue + " ";
        }
        if (!isVisible) str += "INVISIBLE ";
        if (autoIncrement) str += "AUTO_INCREMENT ";
        if (unique) str += "UNIQUE ";
        if (primary) str += "PRIMARY KEY ";
        str += extraDef + " ";
        str.pop_back();
        return str;
    }
};
class DBTable {
    public:

    std::string name;
    std::vector<DBColumn> columns;
    std::string extraDef = "";
};

void mysqli_check_table(MYSQL mysql, std::vector<DBTable> tables) {
    // 不支持修改主键
    writeLog(LOG_LEVEL_INFO, "Checking integrities of MySQL tables...");
    for (int i = 0; i < tables.size(); i++) {
        int primaryCount = 0;
        for (int j = 0; j < tables[i].columns.size(); j++) 
            primaryCount += tables[i].columns[j].primary;
        if (primaryCount > 1) {
            writeLog(LOG_LEVEL_ERROR, "Only 1 primary key can be set in a table");
            return;
        }

        bool exists = mysqli_query(
            mysql, 
            "SHOW TABLES LIKE \"%s\"", 
            tables[i].name.c_str()
        ).size();
        if (!exists) {
            std::string columns = "";
            for (int j = 0; j < tables[i].columns.size(); j++) {
                auto column = tables[i].columns[j];
                if (j) columns += ", ";
                columns += column.toString();
            }
            mysqli_execute(
                mysql, 
                "CREATE TABLE %s (%s) %s", 
                tables[i].name.c_str(),
                columns.c_str(),
                tables[i].extraDef.c_str()
            );
            writeLog(LOG_LEVEL_INFO, "Created table \"%s\"", tables[i].name.c_str());
        } else {
            for (int j = 0; j < tables[i].columns.size(); j++) {
                auto column = tables[i].columns[j];
                bool exists = mysqli_query(
                    mysql,
                    "DESCRIBE %s %s",
                    tables[i].name.c_str(),
                    column.name.c_str()
                ).size();
                column.primary = false;
                if (!exists) {
                    mysqli_execute(
                        mysql,
                        "ALTER TABLE %s ADD COLUMN %s",
                        tables[i].name.c_str(),
                        column.toString().c_str()
                    );
                    writeLog(LOG_LEVEL_INFO, "Added column \"%s\" to table %s", column.name.c_str(), tables[i].name.c_str());
                } else {
                    mysqli_execute(
                        mysql,
                        "ALTER TABLE %s CHANGE %s %s",
                        tables[i].name.c_str(),
                        column.name.c_str(),
                        column.toString().c_str()
                    );
                    // writeLog(LOG_LEVEL_INFO, "Changed column \"%s\" in table %s", column.name.c_str(), tables[i].name.c_str());
                }
            }
        }
    }
}