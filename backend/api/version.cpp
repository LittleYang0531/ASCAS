#include "../ascas.h"
#include "../version.h"

auto Version = [](client_conn conn, http_request request, param argv) {
    Json::Value item;
    item["channel"] = type == "dev" ? channel : "release";
    item["version"] = type == "dev" ? channel + "@" + version : version;
    item["commit"] = commit;
    item["compileTime"] = compileTime;
    item["compileTimestamp"] = compileTimestamp;
    #if defined(__clang__)
    item["compilerType"] = "clang";
    #elif defined(__GNUC__)
    item["compilerType"] = "gcc";
    #elif defined(_MSC_VER)
    item["compilerType"] = "msvc";
    #else
    item["compilerType"] = "unknown";
    #endif
    item["compilerVersion"] = gccVersion;
    item["cppStandard"] = cppStandard;
    quickSendItem(200, item);
};