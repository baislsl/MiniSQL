DROP TABLE user;
CREATE TABLE user (
  user_id       char(20),
  password      char(20),
  register_time char(30),
  mail_address  char(20) unique,
  age           int,
  pow           float,
  PRIMARY KEY (user_id)
);
DESCRIBE user;
INSERT INTO user VALUES ('userid_1', 'password_1', '2017-01-01', '1_baislsl@163.com', 20, 0.5);
INSERT INTO user VALUES ('userid_2', 'password_2', '2017-02-02', '2_baislsl@163.com', 17, 0.45);
INSERT INTO user VALUES ('userid_3', 'password_3', '2017-03-03', '3_baislsl@163.com', 22, 0.77);
INSERT INTO user VALUES ('userid_4', 'password_4', '2017-04-04', '4_baislsl@163.com', 19, 0.9);
INSERT INTO user VALUES ('userid_5', 'password_5', '2017-05-05', '5_baislsl@163.com', 18, 1.0);
INSERT INTO user VALUES ('userid_6', 'password_6', '2017-06-06', '6_baislsl@163.com', 20, 0.3);
INSERT INTO user VALUES ('userid_7', 'password_7', '2017-07-07', '7_baislsl@163.com', 24, 0.2);

# select
SELECT * FROM user;
SELECT user_id, age, pow FROM user WHERE age = 20;
SELECT user_id, age, pow FROM user WHERE age >= 18 and pow > 0.5;

# error test, duplicated primary key and unique mail_address;
INSERT INTO user VALUES ('userid_1', 'password_8', '2017-01-01', '8_baislsl@163.com', 20, 0.5);
INSERT INTO user VALUES ('userid_8', 'password_8', '2017-01-01', '1_baislsl@163.com', 20, 0.5);

# create index 
CREATE INDEX mail_address_index on user( mail_address);
CREATE INDEX password_index on user(password);  	# not unique value -> fail
SELECT * FROM user WHERE mail_address = '1_baislsl@163.com' ;
DROP INDEX mail_address_index;

# delete 
SELECT * FROM user;	# before delete
DELETE FROM user WHERE age < 18;
SELECT * FROM user;  # after delete
DELETE FROM user;
SELECT * FROM user;  # after delete

# quit
# quit;


