CREATE TABLE utilisateurs (
userID INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
dateinscription DATE NOT NULL,
pseudo VARCHAR(40) NOT NULL,
email VARCHAR(80) NOT NULL,
password VARCHAR(255) NOT NULL,
verification tinyint(1) NOT NULL DEFAULT '0',
verificationCode varchar(32) NOT NULL,
lastActivity TIMESTAMP NOT NULL,
INDEX(dateinscription),
UNIQUE(pseudo(40)),
UNIQUE(email(40)))
ENGINE INNODB;


ALTER TABLE utilisateurs AUTO_INCREMENT=10001;