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
INSERT INTO "user" VALUES('testdata','test','test',20,'password');
INSERT INTO "user" VALUES('test','test','test',20,'test');
INSERT INTO "user" VALUES('test1','test','test',20,NULL);
INSERT INTO "user" VALUES('t1','t1','t',2,'sd');
INSERT INTO "user" VALUES('g1','g','g',12,'g');
INSERT INTO "user" VALUES('vinay','vinay','hiwarkar',21,'vinay');
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
        straight REAL,
        cut REAL,
        safety REAL,
        spin REAL,
        power REAL,
        maxpot REAL,
        maxscore REAL,
        overall REAL
);
INSERT INTO "profile" VALUES('ronnie11',73,50,50,50,50,50,50,54);
INSERT INTO "profile" VALUES('testdata',73,50,50,50,50,0,0,54);
INSERT INTO "profile" VALUES('test2',73,50,50,50,50,0,0,54);
INSERT INTO "profile" VALUES('t1',73,50,50,50,50,0,0,54);
INSERT INTO "profile" VALUES('g1',73,50,50,50,50,0,0,54);
INSERT INTO "profile" VALUES('vinay',73,50,50,50,50,0,0,54);
CREATE TABLE shothistory(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        userID TEXT,
        angleerror REAL,
        totaldist REAL,
        totaltime REAL,
        velocity REAL,
	shottype INT
);
INSERT INTO "shothistory" VALUES(1,'ronnie11',1.90685,1285.57,89.0,5.30617,0);
INSERT INTO "shothistory" VALUES(2,'ronnie11',0.474938,2612.27,118.0,8.13228,0);
INSERT INTO "shothistory" VALUES(3,'ronnie11',3.24026,710.865,536.0,0.487191,0);
DELETE FROM sqlite_sequence;
INSERT INTO "sqlite_sequence" VALUES('shothistory',3);
COMMIT;
