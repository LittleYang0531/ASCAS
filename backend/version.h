#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "8f9f5c84f00771bac0bcde5ea761546c549eda66";
const std::string commit = "8f9f5c84f00771bac0bcde5ea761546c549eda66";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
