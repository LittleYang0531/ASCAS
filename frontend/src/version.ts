export var type = "dev";
export var channel = "main";
export var version = "3ec3e1d8f258e5d79d513f3e85e272c2c131d8e1";
export var commit = "3ec3e1d8f258e5d79d513f3e85e272c2c131d8e1";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
