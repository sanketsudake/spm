CREATE TABLE if not exists user(
        userID TEXT PRIMARY KEY NOT NULL,
        first TEXT not null,
        last TEXT not null,
        age INT not null,
        password TEXT
);

CREATE TABLE if not exists frame(
        userID TEXT,
        played INT,
        won INT,
        lost INT,
        PRIMARY KEY (userID),
        FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT
);

CREATE TABLE if not exists matches(
        userID TEXT,
        played INT,
        won INT,
        lost INT,
        PRIMARY KEY (userID),
        FOREIGN KEY (userID) REFERENCES user(userID) ON DELETE RESTRICT
);

CREATE TABLE if not exists profile(
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

CREATE TABLE if not exists shothistory(
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
