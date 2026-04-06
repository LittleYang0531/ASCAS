#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "d276005e114158d58f48989a20f854cfc04a6305";
const std::string commit = "d276005e114158d58f48989a20f854cfc04a6305";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
