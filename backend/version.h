#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "20229fc83754ca4f3e1979667afd500b23cb7d74";
const std::string commit = "20229fc83754ca4f3e1979667afd500b23cb7d74";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
