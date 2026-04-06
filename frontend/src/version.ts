export var type = "dev";
export var channel = "main";
export var version = "cc33eb8eafef1582fb75d515264c3f7045ad837c";
export var commit = "cc33eb8eafef1582fb75d515264c3f7045ad837c";
export var channelString = (type == "dev" ? channel : "release");
export var versionString = (type == "dev" ? channel + "@" + version : version);
