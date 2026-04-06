#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "645a202f71e86dbb0500b9173f0cdd7fddc82d1b";
const std::string commit = "645a202f71e86dbb0500b9173f0cdd7fddc82d1b";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
