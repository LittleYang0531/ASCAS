import type { User } from "./user";

export interface Team {
    tid?: number;
    title?: string;
    description?: string;
    owner?: User;
    createdAt?: number;
    members?: User[];
};