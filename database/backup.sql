PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE user(
    userID TEXT PRIMARY KEY NOT NULL,
    first TEXT not null,
    last TEXT not null,
    age INT not null,
    password TEXT
);
INSERT INTO "user" VALUES('ronnie11','ronnie','patel',35,'password');
INSERT INTO "user" VALUES('tripples','sanket','sudake',21,'password');
INSERT INTO "user" VALUES('sagar','sagar','rakshe',21,'password');
CREATE TABLE frame(
    userID TEXT,
    played INT,
    won INT,
    lost INT,
    PRIMARY KEY (userID),
    FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT

);
CREATE TABLE matches(
    userID TEXT,
    played INT,
    won INT,
    lost INT,
    PRIMARY KEY (userID),
    FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT
);
CREATE TABLE profile(
    userID TEXT PRIMARY KEY,
    straight INT,
    cut INT,
    safety INT,
    spin INT,
    power INT,
    maxpot INT,
    maxscore INT,
    overall INT);
INSERT INTO "profile" VALUES('ronnie11',15,45,63,73,69,50,50,53);

CREATE TABLE if not exists currentshot(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        userID TEXT,
        angleerror REAL,
        totaldist REAL,
        totaltime REAL,
        velocity REAL

);
INSERT INTO "currentshot" (userID,angleerror,totaldist,totaltime,velocity) VALUES('ronnie11',0,0,0,0);
COMMIT;
