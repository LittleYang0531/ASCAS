#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "7521de73b218db5b2b9f0a89c96f1b04b7217150";
const std::string commit = "7521de73b218db5b2b9f0a89c96f1b04b7217150";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
