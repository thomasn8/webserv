CREATE TABLE suggestions (
suggestionNO INT(5) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
datesuggestion DATE NOT NULL,
domaine VARCHAR(40) NOT NULL,
domaineID INT(4) UNSIGNED NOT NULL,
suggestion VARCHAR(40) NOT NULL,
commentaire VARCHAR(80) NOT NULL,
prenom VARCHAR(40) NOT NULL,
email VARCHAR(80) NOT NULL,
UNIQUE(email(20)))
ENGINE INNODB;