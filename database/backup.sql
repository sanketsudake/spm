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
INSERT INTO "user" VALUES('test1','test','test',20,'password');
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
INSERT INTO "profile" VALUES('ronnie11',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('tripples',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('sagar',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('testdata',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('test',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('test1',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('t1',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('g1',0,0,0,0,0,0,0,0);
INSERT INTO "profile" VALUES('vinay',0,0,0,0,0,0,0,0);
CREATE TABLE shothistory(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        userID TEXT,
        angleerror REAL,
        totaldist REAL,
	suggdist REAL,
        totaltime REAL,
        velocity REAL,
	shottype INT,
	accuracy REAL
);
COMMIT;
