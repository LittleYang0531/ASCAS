#pragma once

#include <vector>
#include "../include/mysql.h"

std::vector<DBTable> tables = {
    {
        .name = "users",
        .columns = {
            {
                .name = "id",
                .type = "int",
                .enableDefaultValue = false,
                .autoIncrement = true,
                .primary = true,
            }, {
                .name = "name",
                .type = "text"
            }, {
                .name = "passwd",
                .type = "text"
            }, {
                .name = "email",
                .type = "text"
            }, {
                .name = "verified",
                .type = "boolean",
                .defaultValue = "false",
                .valueIsExpr = true
            }, {
                .name = "isAdmin",
                .type = "boolean",
                .defaultValue = "false",
                .valueIsExpr = true
            }, {
                .name = "description",
                .type = "longtext"
            }
        },
    }, {
        .name = "sessions",
        .columns = {
            {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "session",
                .type = "text"
            }, {
                .name = "expireAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "verifications",
        .columns = {
            {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "code",
                .type = "text"
            }, {
                .name = "expireAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "resetCodes",
        .columns = {
            {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "code",
                .type = "text"
            }, {
                .name = "expireAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "crops",
        .columns = {
            {
                .name = "id",
                .type = "int",
                .enableDefaultValue = false,
                .autoIncrement = true,
                .primary = true,
            }, {
                .name = "name",
                .type = "varchar(255)"
            }, {
                .name = "title",
                .type = "text"
            }, {
                .name = "description",
                .type = "text"
            }, {
                .name = "properties",
                .type = "text"
            }, {
                .name = "owner",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "editors",
                .type = "text"
            }, {
                .name = "viewers",
                .type = "text"
            }, {
                .name = "createdAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "updatedAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "crop_invites",
        .columns = {
            {
                .name = "cid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "code",
                .type = "text",
            }, {
                .name = "isEditor",
                .type = "boolean",
                .defaultValue = "FALSE",
                .valueIsExpr = true
            }, {
                .name = "expiredAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "teams",
        .columns = {
            {
                .name = "id",
                .type = "int",
                .enableDefaultValue = false,
                .autoIncrement = true,
                .primary = true,
            }, {
                .name = "title",
                .type = "text",
            }, {
                .name = "description",
                .type = "text"
            }, {
                .name = "owner",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "createdAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "team_members",
        .columns = {
            {
                .name = "tid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "team_invites",
        .columns = {
            {
                .name = "tid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "code",
                .type = "text",
            }, {
                .name = "expiredAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "user_talks",
        .columns = {
            {
                .name = "uid1",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "uid2",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "team_talks",
        .columns = {
            {
                .name = "tid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "messages",
        .columns = {
            {
                .name = "talkId",
                .type = "text"
            }, {
                .name = "mid",
                .type = "int",
                .enableDefaultValue = false,
                .autoIncrement = true,
                .primary = true,
            }, {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "message",
                .type = "text"
            }, {
                .name = "createdAt",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }, {
        .name = "unread_marks",
        .columns = {
            {
                .name = "mid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }, {
                .name = "uid",
                .type = "int",
                .defaultValue = "0",
                .valueIsExpr = true
            }
        }
    }
};