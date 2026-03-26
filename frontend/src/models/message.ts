import type { User } from "./user";

export enum MessageType {
    Success = "success",
    Info = "info",
    Error = "error",
    Warning = "warning",
};

export interface Message {
    mid?: number,
    user?: User,
    message?: string,
    createdAt?: number
};

export interface Talk {
    title?: string,
    talkId?: string,
    avatar?: string,
    unread?: number,
    latest?: Message
};