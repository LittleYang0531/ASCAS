#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "1a7ecf28b470fb5964920649f0b4437a96d349f4";
const std::string commit = "1a7ecf28b470fb5964920649f0b4437a96d349f4";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
