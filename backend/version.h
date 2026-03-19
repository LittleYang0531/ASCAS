#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "7b3350d16e95230d488ccc3b2c0d331fe1a92a72";
const std::string commit = "7b3350d16e95230d488ccc3b2c0d331fe1a92a72";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
