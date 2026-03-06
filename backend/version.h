#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "9007a606f07d119c298af2021dc3e093fcb05d42";
const std::string commit = "9007a606f07d119c298af2021dc3e093fcb05d42";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
