export var type = "dev";
export var channel = "main";
export var version = "645a202f71e86dbb0500b9173f0cdd7fddc82d1b";
export var commit = "645a202f71e86dbb0500b9173f0cdd7fddc82d1b";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
