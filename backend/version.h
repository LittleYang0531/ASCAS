#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "d088416d721d8673b9fc39dd092af222c9787102";
const std::string commit = "d088416d721d8673b9fc39dd092af222c9787102";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
