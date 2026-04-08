export var type = "dev";
export var channel = "main";
export var version = "d088416d721d8673b9fc39dd092af222c9787102";
export var commit = "d088416d721d8673b9fc39dd092af222c9787102";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
