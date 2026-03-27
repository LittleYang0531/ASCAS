export var type = "dev";
export var channel = "main";
export var version = "e48734918d97c49f077078fba72a3f833c08fd34";
export var commit = "e48734918d97c49f077078fba72a3f833c08fd34";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
