#pragma once

#include <chrono>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <jsoncpp/json/value.h>
#include <string>

#define MyAssert(expr, ...) (void)( \
    (!!(expr)) || \
    (__builtin_MyAssert(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, __VA_ARGS__), 0) \
)

void __builtin_MyAssert(std::string file, int line, std::string func, std::string exprString, const char* format, ...) {
    const int size = 128 * 1024;
    va_list args;
    va_start(args, format);
    char buf[size];
    memset(buf, '\0', sizeof buf);
    int n = vsnprintf(buf, size, format, args);
    va_end(args);

    std::cerr << "Assertion Failed: " << exprString << std::endl;
    std::cerr << "In \033[0;33m" << func << "\033[0m at \033[0;32m" << file << "\033[0m:" << line << std::endl;
    if (strlen(buf) != 0) std::cerr << buf << std::endl;

    abort();
}

time_t clock2() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string readFile(std::string path) {
    std::ifstream fin(path);
    if (!fin) return "";
    fin.seekg(0, std::ios::end);
    int len = fin.tellg();
    if (len == -1) return "";
    fin.seekg(0, std::ios::beg);
    char *ch = new char[len];
    fin.read(ch, len);
    fin.close();
    std::string res = std::string(ch, len);
    delete[] ch;
    return res;
}

void clearFile(std::string path) {
    std::ofstream fout(path);
    fout.close();
}

std::string str_replace(std::string from, std::string to, std::string source) {
    std::string result = source;
	int st = 0, wh = result.find(from.c_str(), st);
	while (wh != std::string::npos) {
        result.replace(wh, from.size(), to.c_str());
		st = wh + to.size();
		wh = result.find(from.c_str(), st);
	} 
    return result;
}

std::string quote_encode(std::string source) {
    return str_replace("\n", "\\n", str_replace("\"", "\\\"", str_replace("\\", "\\\\", source)));
}

std::string sprintf(const char* format, ...) {
    // 格式化文本
    va_list args;
    va_start(args, format);
    char buf[1024 * 1024];
    memset(buf, '\0', sizeof buf);
    int n = vsprintf(buf, format, args);
    va_end(args);
    return buf;
}

std::string trim(std::string str) {
    int st = 0;
    for (; st < str.size(); st++) if (std::string(" \r\n\t\f\t\v").find(str[st]) == std::string::npos) break;
    str = str.substr(st);
    while (str.size() && std::string(" \r\n\t\f\t\v").find(str.back()) != std::string::npos) str.pop_back();
    return str;
}

#define quickSendObject(code, object) [&](){ \
    std::string responseBody = json_encode(object); \
    auto response = __api_default_response; \
    response["Content-Length"] = std::to_string(responseBody.size()); \
    response["Access-Control-Allow-Origin"] = request.argv["origin"]; \
    putRequest(conn, code, response); \
    if (dbConnected) quick_mysqli_close(); \
    send(conn, responseBody); \
    exitRequest(conn); \
}()

#define quickSendCode(code) [&](){ \
    Json::Value object; \
    object["code"] = code; \
    object["msg"] = http_code[code]; \
    quickSendObject(code, object); \
}()

#define quickSendItem(code, item) [&](){ \
    Json::Value object; \
    object["code"] = code; \
    object["msg"] = http_code[code]; \
    object["item"] = item; \
    quickSendObject(code, object); \
}()

#define quickSendItems(code, items) [&](){ \
    Json::Value object; \
    object["code"] = code; \
    object["msg"] = http_code[code]; \
    object["items"] = items; \
    quickSendObject(code, object); \
}()

#define quickSendData(code, ...) [&](){ \
    Json::Value object; \
    object["code"] = code; \
    object["msg"] = http_code[code]; \
    __mybuiltin_quickSetData(object, # __VA_ARGS__, ## __VA_ARGS__); \
    quickSendObject(code, object); \
}()

void __mybuiltin_quickSetData(Json::Value &object, std::string expr) {}

template<typename T, typename ...Args>
void __mybuiltin_quickSetData(Json::Value &object, std::string expr, T value, Args ...args) {
    std::string name = "";
    if (expr.find(",") != std::string::npos) {
        name = trim(expr.substr(0, expr.find(",")));
        expr = expr.substr(expr.find(",") + 1);
    } else {
        name = trim(expr);
    }
    object[name] = value;
    __mybuiltin_quickSetData(object, expr, args...);
}