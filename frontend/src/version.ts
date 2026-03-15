export var type = "dev";
export var channel = "main";
export var version = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
export var commit = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
