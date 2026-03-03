#include "ascas.h"

#include "api/notFound.cpp"
#include "api/favicon.cpp"
#include "api/users/check.cpp"
#include "api/users/login.cpp"
#include "api/users/create.cpp"
#include "api/users/verify.cpp"
#include "api/users/reset.cpp"

int main(int argc, char** argv) {
    // Log.logLevelId = LOG_LEVEL_DEBUG;

    std::string proc_name = argv[0];
    proc_daemon(argv, "ascas-backend: daemon process", { "config.json" });
    proc_settitle(("ascas-backend: master process " + proc_name).c_str());
    
    appConfig = json_decode(readFile("./config.json"));
    app.setopt(HTTP_ENABLE_SSL, appConfig["server.enableSSL"].asBool());
    app.setopt(HTTP_LISTEN_HOST, appConfig["server.listenHost"].asCString());
    app.setopt(HTTP_LISTEN_PORT, appConfig["server.listenPort"].asInt());
    app.setopt(HTTP_SSL_CACERT, appConfig["server.httpsCacert"].asCString());
    app.setopt(HTTP_SSL_PRIVKEY, appConfig["server.httpsPrivkey"].asCString());
    app.setopt(HTTP_MULTI_THREAD, appConfig["server.threadNumber"].asInt());
    app.setopt(HTTP_WORKER_TITLE, "ascas-backend: http worker process");
    app.setopt(HTTP_WS_WORKER_TITLE, "ascas-backend: websocket worker process");

    app.addRoute("/users/check", UsersCheck);
    app.addRoute("/users/login", UsersLogin);
    app.addRoute("/users/create", UsersCreate);
    app.addRoute("/users/verify", UsersVerify);
    app.addRoute("/users/reset", UsersReset);
    app.addRoute("/favicon.ico", Favicon);
    app.addRoute("*", NotFound);
    __default_response["Access-Control-Allow-Credentials"] = "true";
    __default_response["Access-Control-Allow-Headers"] = "*";
    __default_response["Access-Control-Allow-Methods"] = "*";
    app.run();
}