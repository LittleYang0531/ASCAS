#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "70f9f6f88c29d581d0f267143d5f35d34f9eeb99";
const std::string commit = "70f9f6f88c29d581d0f267143d5f35d34f9eeb99";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
