export var type = "dev";
export var channel = "main";
export var version = "70f9f6f88c29d581d0f267143d5f35d34f9eeb99";
export var commit = "70f9f6f88c29d581d0f267143d5f35d34f9eeb99";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
