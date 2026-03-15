export const type = "dev";
export const channel = "main";
export const version = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
export const commit = "f14b18a6c4cf09f1aa4563fda5f4790b10b2789a";
export const channelString = (type == "dev" ? channel : "release");
export const versionString = (type == "dev" ? channel + "@" + version : version);
