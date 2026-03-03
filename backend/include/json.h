#pragma once

#include <cstdio>
#include <jsoncpp/json/value.h>
#include<string>
#include<jsoncpp/json/json.h>

std::string json_encode(Json::Value val) {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream os;
    writer->write(val, &os);
    return os.str();
}

std::string json_pretty_encode(Json::Value val) {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "    ";
    builder["emitUTF8"] = true;
    builder["enableYAMLCompatibility"] = true;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream os;
    writer->write(val, &os);
    return os.str();
}

bool json_decode(std::string json, Json::Value& res) {
    Json::Reader reader;
    return reader.parse(json, res);
}

Json::Value json_decode(std::string json) {
    Json::Value res;
    Json::Reader reader;
    reader.parse(json, res);
    return res;
}

Json::Value __my_builtin_jsonobj() {
    return Json::objectValue;
}

template<typename T, typename ...Args>
Json::Value __my_builtin_jsonobj(std::string key, T value, Args ...args) {
    Json::Value res = __my_builtin_jsonobj(args...);
    res[key] = value;
    return res;
}

template<typename ...Args>
Json::Value jsonobj(Args ...args) { return __my_builtin_jsonobj(args...); }

Json::Value __my_builtin_jsonarr() {
    return Json::arrayValue;
}

template<typename T, typename ...Args>
Json::Value __my_builtin_jsonarr(T value, Args ...args) {
    Json::Value res = __my_builtin_jsonarr(args...);
    res.insert(0, value);
    return res;
}

template<typename ...Args>
Json::Value jsonarr(Args ...args) { return __my_builtin_jsonarr(args...); }