export var type = "dev";
export var channel = "main";
export var version = "7521de73b218db5b2b9f0a89c96f1b04b7217150";
export var commit = "7521de73b218db5b2b9f0a89c96f1b04b7217150";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
