export var type = "dev";
export var channel = "main";
export var version = "d276005e114158d58f48989a20f854cfc04a6305";
export var commit = "d276005e114158d58f48989a20f854cfc04a6305";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
