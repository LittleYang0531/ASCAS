#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "c379bffa5d80ed48da413acf45251a8d47a58489";
const std::string commit = "c379bffa5d80ed48da413acf45251a8d47a58489";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
