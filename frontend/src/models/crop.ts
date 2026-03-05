import type { User } from "./user";

export interface RecordProperty {
    name?: string;
    title?: string;
    unit?: string;
    required?: boolean;
    def?: string;
    options?: Array<string>;
    type?: string;
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
    updatedAt?: number;
    permission?: string;
};