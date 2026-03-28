#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "3ec3e1d8f258e5d79d513f3e85e272c2c131d8e1";
const std::string commit = "3ec3e1d8f258e5d79d513f3e85e272c2c131d8e1";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
