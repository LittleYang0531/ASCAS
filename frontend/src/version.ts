export const type = "dev";
export const channel = "main";
export const version = "4b7a21cfbb7b67f68869146c7931bc298b119955";
export const commit = "4b7a21cfbb7b67f68869146c7931bc298b119955";
export const channelString = (type == "dev" ? channel : "release");
export const versionString = (type == "dev" ? channel + "@" + version : version);
