export var type = "dev";
export var channel = "main";
export var version = "98928e602078b6765b861243174babaa75ce333e";
export var commit = "98928e602078b6765b861243174babaa75ce333e";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
