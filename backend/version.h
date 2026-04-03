#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "98928e602078b6765b861243174babaa75ce333e";
const std::string commit = "98928e602078b6765b861243174babaa75ce333e";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
