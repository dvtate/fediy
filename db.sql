CREATE DATABASE gitfed;


CREATE TABLE Users (
    `user_id` BIGINT PRIMARY KEY,
    `username` VARCHAR(128) UNIQUE NOT NULL,
    `password` CHAR(128) NOT NULL,
    `email` CHAR(128) NOT NULL,
);

CREATE TABLE Repo (
    
);