#pragma once

// 资源文件
#include "resources/config_json.h"
#include "resources/favicon_ico.h"
#include "resources/reset_html.h"
#include "resources/signup_html.h"
#include "resources/invite_team_md.h"
#include "resources/invite_crop_md.h"
#include "resources/system_channels.h"
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
#include "include/socket.h"

#include "models/user.cpp"
#include "models/crop.cpp"
#include "models/record.cpp"
#include "models/team.cpp"
#include "models/message.cpp"
#include "models/blog.cpp"

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
#include "utils/record.cpp"
#include "utils/team.cpp"
#include "utils/message.cpp"
#include "utils/blog.cpp"