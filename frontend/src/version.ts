export var type = "dev";
export var channel = "main";
export var version = "56cc224bdd172ea89c3612a17dcd5d2023ccbfc0";
export var commit = "56cc224bdd172ea89c3612a17dcd5d2023ccbfc0";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
