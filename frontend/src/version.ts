export var type = "dev";
export var channel = "main";
export var version = "33737ee87c53d16640263f0e65221d4ea7b1ab35";
export var commit = "33737ee87c53d16640263f0e65221d4ea7b1ab35";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
