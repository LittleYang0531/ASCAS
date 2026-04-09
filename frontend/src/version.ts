export var type = "dev";
export var channel = "main";
export var version = "eaa11b15716fd401d9b56a8e3f4e2a621309a613";
export var commit = "eaa11b15716fd401d9b56a8e3f4e2a621309a613";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
