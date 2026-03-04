#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> EnumToName;
std::map<std::string, int> NameToEnum;

template<typename T, T n>
std::string __mybuiltin_enum_getNameFromEnum() {
    std::string name = __PRETTY_FUNCTION__;
    std::string value = name.substr(name.find("T n = ") + 6);
    value = value.substr(0, std::min(value.find(";"), value.find("]")));
    return value;
}

template<typename T>
std::string getNameFromEnum(T value) {
    return EnumToName[std::string(typeid(T).name()) + "." + std::to_string((int)value)];
}

template<typename T>
T __mybuiltin_enum_getEnumFromName(std::string name) {
    return (T)NameToEnum[name];
}
#define getEnumFromName(T, name) __mybuiltin_enum_getEnumFromName<T>(name)

template<typename T, int Begin, int End>
void __mybuiltin_enum_initEnum() {
    static_assert(
        End - Begin <= 1024,
        "\n"
        "    Enum range is too large for efficient compilation.\n"
        "    If this is a bit-operation enum, please use `initEnumBin` instead.\n"
        "    If the enum size is indeed large, please use `initEnumLarge` instead."
    );
    EnumToName[std::string(typeid(T).name()) + "." + std::to_string((int)Begin)] = __mybuiltin_enum_getNameFromEnum<T, (T)Begin>();
    NameToEnum[__mybuiltin_enum_getNameFromEnum<T, (T)Begin>()] = Begin;
    if constexpr (Begin < End) {
        __mybuiltin_enum_initEnum<T, Begin + 1, End>();
    } else ;
}
#define initEnum(T, begin, end) bool __unused_ ## T ## _unused__ = [](){ __mybuiltin_enum_initEnum<T, (int)T::begin, (int)T::end>(); return true; }()

template<typename T, int Begin, int End>
void __mybuiltin_enum_initEnumLarge() {
    EnumToName[std::string(typeid(T).name()) + "." + std::to_string((int)Begin)] = __mybuiltin_enum_getNameFromEnum<T, (T)Begin>();
    NameToEnum[__mybuiltin_enum_getNameFromEnum<T, (T)Begin>()] = Begin;
    if constexpr (Begin < End) {
        __mybuiltin_enum_initEnumLarge<T, Begin + 1, End>();
    } else ;
}
#define initEnumLarge(T, begin, end) bool __unused_ ## T ## _unused__ = [](){ __mybuiltin_enum_initEnumLarge<T, (int)T::begin, (int)T::end>(); return true; }()

template<typename T, int Begin, int End>
void __mybuiltin_enum_initEnumBin() {
    EnumToName[std::string(typeid(T).name()) + "." + std::to_string((int)Begin)] = __mybuiltin_enum_getNameFromEnum<T, (T)Begin>();
    NameToEnum[__mybuiltin_enum_getNameFromEnum<T, (T)Begin>()] = Begin;
    if constexpr (Begin < End) {
        __mybuiltin_enum_initEnumBin<T, Begin << 1, End>();
    } else ;
}
#define initEnumBin(T, begin, end) bool __unused_ ## T ## _unused__ = [](){ __mybuiltin_enum_initEnumBin<T, (int)T::begin, (int)T::end>(); return true; }()