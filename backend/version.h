#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
const std::string commit = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
