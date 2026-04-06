#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "7c7b6e1e0ea15aca88693ca386b7041171fbd92a";
const std::string commit = "7c7b6e1e0ea15aca88693ca386b7041171fbd92a";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
