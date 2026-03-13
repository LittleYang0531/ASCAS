#pragma once

// 资源文件
#include "resources/config_json.h"
#include "resources/favicon_ico.h"
#include "resources/reset_html.h"
#include "resources/signup_html.h"
#include "resources/tables.h"

#include "include/encrypt.h"
#include "include/httpd.h"
#include "include/json.h"
#include "include/log.h"
#include "include/mysql.h"
#include "include/proc.h"
#include "include/utils.h"
#include "include/smtp.h"
#include "include/png.h"

#include "models/user.cpp"
#include "models/crop.cpp"

Json::Value appConfig;
MYSQL mysql;
bool dbConnected = false;
void quick_mysqli_connect() {
    if (dbConnected) return;
    mysql = mysqli_connect(
        appConfig["mysql.host"].asString(),
        appConfig["mysql.user"].asString(),
        appConfig["mysql.passwd"].asString(),
        appConfig["mysql.database"].asString(),
        appConfig["mysql.port"].asInt()
    );
    dbConnected = true;
}
void quick_mysqli_close() {
    if (!dbConnected) return;
    mysqli_close(mysql);
    dbConnected = false;
}

#include "utils/user.cpp"
#include "utils/crop.cpp"