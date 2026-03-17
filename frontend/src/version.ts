export var type = "dev";
export var channel = "main";
export var version = "8a1f7ef2697aa65a46246758e40cd8a5bb99f42d";
export var commit = "8a1f7ef2697aa65a46246758e40cd8a5bb99f42d";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
