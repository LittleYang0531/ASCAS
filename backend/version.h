#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "4e60558ecfe8209cb2298fedc5abc9a8c736df40";
const std::string commit = "4e60558ecfe8209cb2298fedc5abc9a8c736df40";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
