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
        straight INT,
        cut INT,
        safety INT,
        spin INT,
        power INT,
        maxpot INT,
        maxscore INT,
        overall INT
);

CREATE TABLE if not exists currentshot(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        userID TEXT,
        angleerror REAL,
        totaldist REAL,
        totaltime REAL,
        velocity REAL

);
