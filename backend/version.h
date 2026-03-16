#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "02afc8a6bc086cd1f314956d16a54f4edc73f20c";
const std::string commit = "02afc8a6bc086cd1f314956d16a54f4edc73f20c";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
