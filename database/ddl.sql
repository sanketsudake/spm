CREATE TABLE if not exists user(
    userID varchar(20) PRIMARY KEY,
    first varchar(20) not null,
    last varchar(20) not null,
    age integer not null,
    password varchar(150)
);


CREATE TABLE if not exists frame(
    userID varchar(20),
    played integer,
    won integer,
    lost integer,
    PRIMARY KEY (userID),
    FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT

);

CREATE TABLE if not exists matches(
    userID varchar(20),
    played integer,
    won integer,
    lost integer,
    PRIMARY KEY (userID),
    FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT
);

CREATE TABLE if not exists profile(
    userID varchar(20) PRIMARY KEY,
    straight integer,
    cut integer,
    safety integer,
    spin integer,
    power integer,
    maxpot integer,
    maxscore integer,
    overall integer
);


