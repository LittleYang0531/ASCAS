import type { User } from "./user";

export interface Blog {
    bid?: number;
    author?: User;
    title?: string;
    content?: string;
    createdAt?: number;
    likes?: number;
    dislikes?: number;
    stars?: number;
    comments?: number;
    images?: string[];
};

export interface Comment {
    cid?: number;
    author?: User;
    comment?: string;
    likes?: number;
    dislikes?: number;
    liked?: boolean;
    disliked?: boolean;
    createdAt?: number;
};