export interface User {
    uid?: number;
    name?: string;
    email?: string;
    isAdmin?: boolean;
}

export function mergeUsers(...users: User[][]): User[] {
    const map = new Map<number, User>();
    for (const userList of users)
        for (const user of userList)
            if (!map.has(user.uid!))
                map.set(user.uid!, user);
    return Array.from(map.values());
}