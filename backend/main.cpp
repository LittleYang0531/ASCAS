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

initEnum(LOG_LEVEL, LOG_LEVEL_DEBUG, LOG_LEVEL_ASSERT);
initEnumBin(LOG_TARGET, LOG_TARGET_FILE, LOG_TARGET_CONSOLE);

int main(int argc, char** argv) {
    // Log.logLevelId = LOG_LEVEL_DEBUG;

    std::string proc_name = argv[0];
    proc_daemon(argv, "ascas-backend: daemon process", { "config.json" });
    proc_settitle(("ascas-backend: master process " + proc_name).c_str());
    
    appConfig = json_decode(readFile("./config.json"));

    Log.logLevelId = getEnumFromName(LOG_LEVEL, appConfig["log.level"].asString());
    Log.target = (LOG_TARGET)0;
    for (int i = 0; i < appConfig["log.target"].size(); i++) 
        Log.target = LOG_TARGET(Log.target | getEnumFromName(LOG_TARGET, appConfig["log.target"][i].asString()));
    Log.targetPath = appConfig["log.file"].asString();

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