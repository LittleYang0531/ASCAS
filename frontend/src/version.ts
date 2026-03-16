export var type = "dev";
export var channel = "main";
export var version = "02afc8a6bc086cd1f314956d16a54f4edc73f20c";
export var commit = "02afc8a6bc086cd1f314956d16a54f4edc73f20c";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
