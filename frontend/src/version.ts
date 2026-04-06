export var type = "dev";
export var channel = "main";
export var version = "7c7b6e1e0ea15aca88693ca386b7041171fbd92a";
export var commit = "7c7b6e1e0ea15aca88693ca386b7041171fbd92a";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
