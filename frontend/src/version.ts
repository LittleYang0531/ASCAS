export var type = "dev";
export var channel = "main";
export var version = "0fa8efe4a6bd5799c5a7121a673315eb711159b7";
export var commit = "0fa8efe4a6bd5799c5a7121a673315eb711159b7";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
