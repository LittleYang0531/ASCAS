#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "0fa8efe4a6bd5799c5a7121a673315eb711159b7";
const std::string commit = "0fa8efe4a6bd5799c5a7121a673315eb711159b7";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
