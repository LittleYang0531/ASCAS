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
#include "api/crops/list.cpp"
#include "api/crops/details.cpp"
#include "api/crops/create.cpp"
#include "api/notFound.cpp"

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
    app.addRoute("/crops/list", CropsList);
    app.addRoute("/crops/%d", CropsDetails);
    app.addRoute("/crops/create", CropsCreate);
    app.addRoute("*", NotFound);
    __default_response["Access-Control-Allow-Credentials"] = "true";
    __default_response["Access-Control-Allow-Headers"] = "*";
    __default_response["Access-Control-Allow-Methods"] = "*";
    app.run();
}