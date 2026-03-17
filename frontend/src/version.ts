export var type = "dev";
export var channel = "main";
export var version = "c4af416194c6e7abfc79ccc9004adddff8c5453a";
export var commit = "c4af416194c6e7abfc79ccc9004adddff8c5453a";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
