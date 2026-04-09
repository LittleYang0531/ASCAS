#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "eaa11b15716fd401d9b56a8e3f4e2a621309a613";
const std::string commit = "eaa11b15716fd401d9b56a8e3f4e2a621309a613";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
