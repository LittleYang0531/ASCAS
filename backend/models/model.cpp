#pragma once

#include "enum.h"
#include <functional>
#include <jsoncpp/json/value.h>
#include <vector>

template<typename T, typename Func = std::function<T(Json::Value)> >
std::vector<T> extarr(Json::Value arr, Func extfunc = [](Json::Value obj){ return obj.as<T>(); }) {
    std::vector<T> v;
    for (int i = 0; i < arr.size(); i++) v.push_back(extfunc(arr[i]));
    return v;
}
template<typename T, typename Func = std::function<Json::Value(T)> >
Json::Value packarr(std::vector<T> arr, Func packfunc = [](T obj){ return Json::Value(obj); }) {
    Json::Value res;
    res.resize(0);
    for (int i = 0; i < arr.size(); i++)
        res.append(packfunc(arr[i]));
    return res;
}

#define initModel(name) \
class name : public name ## Base { \
    public: \
 \
    static name fromJsonObject(Json::Value obj) { \
        name ## Base base = name ## Base::fromJsonObject(obj); \
        return *(name*)&base; \
    } \
 \
    static Json::Value toJsonObject(name obj) { \
        return Json::Value(obj); \
    } \
 \
    operator Json::Value() { return name ## Base::toJsonObject(); } \
}; \
template<> \
inline name Json::Value::as<name>() const { \
    return name::fromJsonObject(*this); \
}
