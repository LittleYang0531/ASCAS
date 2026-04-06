#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "cc33eb8eafef1582fb75d515264c3f7045ad837c";
const std::string commit = "cc33eb8eafef1582fb75d515264c3f7045ad837c";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
