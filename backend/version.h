#pragma once

#include <string>

const std::string type = "dev";
const std::string channel = "main";
const std::string version = "2af5711af7e0667ee1fd11b185e51016f855d066";
const std::string commit = "2af5711af7e0667ee1fd11b185e51016f855d066";
const std::string compileTime = __DATE__ " " __TIME__;
const std::string compileTimestamp = __TIMESTAMP__;
const std::string gccVersion = __VERSION__;
const std::string cppStandard = "C++" + std::to_string(__cplusplus).substr(2, 2);
