#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "3e3d05367c75508c56e5fef5caf8b0bc09cc9aa1";
const std::string commit = "3e3d05367c75508c56e5fef5caf8b0bc09cc9aa1";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
