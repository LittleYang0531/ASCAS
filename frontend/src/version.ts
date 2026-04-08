export var type = "dev";
export var channel = "main";
export var version = "8f9f5c84f00771bac0bcde5ea761546c549eda66";
export var commit = "8f9f5c84f00771bac0bcde5ea761546c549eda66";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
