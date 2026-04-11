export var type = "dev";
export var channel = "main";
export var version = "3e3d05367c75508c56e5fef5caf8b0bc09cc9aa1";
export var commit = "3e3d05367c75508c56e5fef5caf8b0bc09cc9aa1";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
