#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "c37d4d08dd1a84336256cb49319ca3739f88d61e";
const std::string commit = "c37d4d08dd1a84336256cb49319ca3739f88d61e";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
