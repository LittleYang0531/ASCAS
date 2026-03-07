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
                .defaultValue = "0",
                .valueIsExpr = true,
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
                .defaultValue = "0",
                .valueIsExpr = true,
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
    }
};