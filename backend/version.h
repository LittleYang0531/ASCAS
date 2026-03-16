#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "c4af416194c6e7abfc79ccc9004adddff8c5453a";
const std::string commit = "c4af416194c6e7abfc79ccc9004adddff8c5453a";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
