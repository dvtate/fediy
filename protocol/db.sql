CREATE DATABASE fediy;

CREATE TABLE Peers (
    domain VARCHAR(64) PRIMARY KEY,
    connectTs BIGINT NOT NULL,
    bearerToken CHAR(24),
    symmetricKey BLOB,
    pubkey TEXT
    tokenExpireTs BIGINT
);

CREATE TABLE Users (
    username VARCHAR(32) PRIMARY KEY,
    hashedPassword CHAR(128) NOT NULL,
    contact TEXT
);

