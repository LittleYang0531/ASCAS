import type { User } from "./user";

export interface Sensor {
    sid?: number,
    owner?: User,
    title?: string,
    unit?: string,
    data?: number[],
    createdAt?: number[]
}