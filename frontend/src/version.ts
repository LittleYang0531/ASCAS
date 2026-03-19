export var type = "dev";
export var channel = "main";
export var version = "7b3350d16e95230d488ccc3b2c0d331fe1a92a72";
export var commit = "7b3350d16e95230d488ccc3b2c0d331fe1a92a72";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
