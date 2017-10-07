/*CREATE TABLE places(*/
CREATE VIRTUAL TABLE placesFts USING fts4(
    country_code CHAR(2),
    postal_code VARCHAR(20),
    place_name VARCHAR(180),
    admin_name1 VARCHAR(100),
    admin_code1 VARCHAR(20),
    admin_name2 VARCHAR(100),
    admin_code2 VARCHAR(20),
    admin_name3 VARCHAR(100),
    admin_code3 VARCHAR(20),
    latitude REAL,
    longitude REAL,
    accuracy INT
);

/*
$ sqlite3 mashup.db
.separator "\t"
.import US.txt places

wc -l US.txt

SELECT COUNT(*) FROM places;

--- hips:
CREATE TABLE words_(word VARCHAR(100));
.import muw.dict words_;
CREATE TABLE words(id INTEGER PRIMARY KEY AUTOINCREMENT, word VARCHAR(100));
INSERT INTO words(word) SELECT * FROM words_;
DROP TABLE words_;
*/
