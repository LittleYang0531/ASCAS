#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "e48734918d97c49f077078fba72a3f833c08fd34";
const std::string commit = "e48734918d97c49f077078fba72a3f833c08fd34";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
