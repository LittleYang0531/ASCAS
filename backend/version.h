#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "56cc224bdd172ea89c3612a17dcd5d2023ccbfc0";
const std::string commit = "56cc224bdd172ea89c3612a17dcd5d2023ccbfc0";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
