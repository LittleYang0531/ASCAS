CREATE TABLE users (
    id int primary key auto_increment,
    name text,
    passwd text,
    email text,
    verified boolean,
    isAdmin boolean
);

CREATE TABLE sessions (
    uid int,
    session text,
    expireAt int
);

CREATE TABLE verifications (
    uid int,
    code text,
    expireAt int
);

CREATE TABLE resetCodes (
    uid int,
    code text,
    expireAt int
);

CREATE TABLE crops (
    id int primary key auto_increment,
    name varchar(255),
    title text,
    description text,
    properties text,
    owner int,
    editors text,
    viewers text,
    createdAt int,
    updatedAt int
);