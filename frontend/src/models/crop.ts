import type { User } from "./user";

export enum UserPermission {
    NONE,
    VIEWER,
    EDITOR,
    OWNER
};

export enum RecordPropertyType {
    NUMBER,
    STRING,
    IMAGE
};

export interface RecordProperty {
    name?: string;
    title?: string;
    unit?: string;
    required?: boolean;
    def?: string;
    type?: RecordPropertyType;
};

export interface Crop {
    cid?: number;
    name?: string;
    title?: string;
    description?: string;
    properties?: RecordProperty[];
    owner?: User;
    editors?: Array<User>;
    viewers?: Array<User>;
    createdAt?: number;
    permission?: UserPermission;
};