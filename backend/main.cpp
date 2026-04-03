#if __cplusplus < 202002L
#error "Please using C++20 or later to compile ASCAS backend!"
#endif

#include "ascas.h"

#include "api/version.cpp"
#include "api/favicon.cpp"
#include "api/users/check.cpp"
#include "api/users/login.cpp"
#include "api/users/create.cpp"
#include "api/users/verify.cpp"
#include "api/users/reset.cpp"
#include "api/users/search.cpp"
#include "api/users/avatar.cpp"
#include "api/users/generateAvatar.cpp"
#include "api/crops/list.cpp"
#include "api/crops/details.cpp"
#include "api/crops/create.cpp"
#include "api/crops/edit.cpp"
#include "api/crops/remove.cpp"
#include "api/records/add.cpp"
#include "api/records/uploadImages.cpp"
#include "api/records/images.cpp"
#include "api/records/list.cpp"
#include "api/records/edit.cpp"
#include "api/records/remove.cpp"
#include "api/teams/create.cpp"
#include "api/teams/list.cpp"
#include "api/teams/details.cpp"
#include "api/teams/edit.cpp"
#include "api/teams/remove.cpp"
#include "api/teams/avatar.cpp"
#include "api/teams/invite.cpp"
#include "api/messages/list.cpp"
#include "api/messages/send.cpp"
#include "api/messages/details.cpp"
#include "api/messages/info.cpp"
#include "api/system/avatar.cpp"
#include "api/notFound.cpp"

#include "ws/broadcast.cpp"
#include "ws/messages/unread.cpp"
#include "ws/messages/list.cpp"
#include "ws/messages/details.cpp"

using Json::ValueType;
initEnum(ValueType, nullValue, objectValue);
initEnum(LOG_LEVEL, LOG_LEVEL_DEBUG, LOG_LEVEL_ASSERT);
initEnumBin(LOG_TARGET, LOG_TARGET_FILE, LOG_TARGET_CONSOLE);

int main(int argc, char** argv) {
    // Log.logLevelId = LOG_LEVEL_DEBUG;
    mkdir("backup", 0777);
    mkdir("data", 0777);
    mkdir("data/avatars", 0777);
    mkdir("data/images", 0777);
    mkdir("data/team-avatars", 0777);
    mkdir("/tmp/ascas", 0777);

    std::string proc_name = argv[0];
    std::vector<std::string> hotreloads = { "config.json" };
    std::vector<std::string> recompiles = { "recompile.json" };
    proc_daemon(argv, "ascas-backend: daemon process", hotreloads, recompiles);
    proc_settitle(("ascas-backend: master process " + proc_name).c_str());
    
    appConfig = json_decode(config_json);
    Json::Value newAppConfig = json_decode(readFile("./config.json"));
    auto members = newAppConfig.getMemberNames();
    for (auto key : members) {
        if (!appConfig.isMember(key)) {
            writeLog(LOG_LEVEL_WARNING, "Invalid config: no such key named \"%s\"", key.c_str());
            continue;
        }
        if (appConfig[key].type() != newAppConfig[key].type()) {
            writeLog(
                LOG_LEVEL_WARNING,
                "Invalid config: \"%s\" expected \"%s\", but \"%s\" was found.",
                key.c_str(),
                getNameFromEnum(appConfig[key].type()).c_str(),
                getNameFromEnum(newAppConfig[key].type()).c_str()
            );
            continue;
        }
        appConfig[key] = newAppConfig[key];
    } 

    Log.logLevelId = getEnumFromName(LOG_LEVEL, appConfig["log.level"].asString());
    Log.target = (LOG_TARGET)0;
    for (int i = 0; i < appConfig["log.target"].size(); i++) 
        Log.target = LOG_TARGET(Log.target | getEnumFromName(LOG_TARGET, appConfig["log.target"][i].asString()));
    Log.targetPath = appConfig["log.file"].asString();

    quick_mysqli_connect();
    std::string backupFile = "./backup/mysql_" + std::to_string(time(NULL)) + ".sql";
    std::string command = "mysqldump";
    if (appConfig["mysql.user"].asString() != "") command +=  " -u " + appConfig["mysql.user"].asString();
    if (appConfig["mysql.passwd"].asString() != "") command += " -p" + appConfig["mysql.passwd"].asString();
    if (appConfig["mysql.host"].asString() != "") command += " -h " + appConfig["mysql.host"].asString();
    command += " -P " + appConfig["mysql.port"].asString();
    command += " " + appConfig["mysql.database"].asString();
    command += " > " + backupFile;
    if (system(command.c_str())) {
        writeLog(LOG_LEVEL_ERROR, "Failed to back up MySQL");
        return 3;
    }
    writeLog(LOG_LEVEL_INFO, "Backed up MySQL to \"%s\"", backupFile.c_str());
    mysqli_check_table(mysql, tables);
    quick_mysqli_close();

    pid_t pid;
    proc_create(&pid, generateBroadcastServer("/tmp/ascas/msgUnread.sock"), NULL);
    proc_create(&pid, generateBroadcastServer("/tmp/ascas/msgList.sock"), NULL);
    proc_create(&pid, generateBroadcastServer("/tmp/ascas/msgDetails.sock"), NULL);

    app.setopt(HTTP_ENABLE_SSL, appConfig["server.enableSSL"].asBool());
    app.setopt(HTTP_LISTEN_HOST, appConfig["server.listenHost"].asCString());
    app.setopt(HTTP_LISTEN_PORT, appConfig["server.listenPort"].asInt());
    app.setopt(HTTP_SSL_CACERT, appConfig["server.httpsCacert"].asCString());
    app.setopt(HTTP_SSL_PRIVKEY, appConfig["server.httpsPrivkey"].asCString());
    app.setopt(HTTP_MULTI_THREAD, appConfig["server.threadNumber"].asInt());
    app.setopt(HTTP_WORKER_TITLE, "ascas-backend: http worker process");
    app.setopt(HTTP_WS_WORKER_TITLE, "ascas-backend: websocket worker process");

    app.addRoute("/version", Version);
    app.addRoute("/favicon.ico", Favicon);
    app.addRoute("/users/check", UsersCheck);
    app.addRoute("/users/login", UsersLogin);
    app.addRoute("/users/create", UsersCreate);
    app.addRoute("/users/verify", UsersVerify);
    app.addRoute("/users/reset", UsersReset);
    app.addRoute("/users/search", UsersSearch);
    app.addRoute("/users/generateAvatar", UsersGenerateAvatar);
    app.addRoute("/users/%d/avatar", UsersAvatar);
    app.addRoute("/crops/list", CropsList);
    app.addRoute("/crops/%d", CropsDetails);
    app.addRoute("/crops/create", CropsCreate);
    app.addRoute("/crops/%d/edit",CropsEdit);
    app.addRoute("/crops/%d/records/add",CropsRecordsAdd);
    app.addRoute("/crops/%d/images", RecordsUploadImages);
    app.addRoute("/crops/%d/images/%s", RecordsImages);
    app.addRoute("/crops/%d/records/list",CropsRecordsList);
    app.addRoute("/crops/%d/records/%d/edit",CropsRecordsEdit);
    app.addRoute("/crops/%d/records/%d/remove",CropsRecordRemove);
    app.addRoute("/crops/%d/remove",CropsRemove);
    app.addRoute("/teams/create",TeamsCreate);
    app.addRoute("/teams/%d", TeamsDetails);
    app.addRoute("/teams/list",TeamsList);
    app.addRoute("/teams/%d/edit",TeamsEdit);
    app.addRoute("/teams/%d/remove",TeamsRemove);
    app.addRoute("/teams/%s/avatar", TeamsAvatar);
    app.addRoute("/teams/%d/invite", TeamsInvite);
    app.addRoute("/messages/list", MessagesList);
    app.addRoute("/messages/send", MessagesSend);
    app.addRoute("/messages/%s", MessagesDetails);
    app.addRoute("/messages/%s/info", MessagesInfo);
    app.addRoute("/system/avatar/%s", SystemAvatar);
    app.addRoute("*", NotFound);

    app.ws_addRoute("/messages/unread", WSMessagesUnread);
    app.ws_addRoute("/messages/list/websocket", WSMessagesList);
    app.ws_addRoute("/messages/%s/websocket", WSMessagesDetails);

    __default_response["Access-Control-Allow-Credentials"] = "true";
    __default_response["Access-Control-Allow-Headers"] = "*";
    __default_response["Access-Control-Allow-Methods"] = "*";
    app.run();
}