#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "828e6b475fb676de68c920b06d6224b32478ca8c";
const std::string commit = "828e6b475fb676de68c920b06d6224b32478ca8c";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
