CREATE TABLE conversations (
conversationNO INT(10) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
annonceNO INT(10) UNSIGNED NOT NULL,
matiere VARCHAR(60) NOT NULL,
dateDebut DATE NOT NULL,
annonceur VARCHAR(40) NOT NULL,
repondeur VARCHAR(40) NOT NULL,
lastMessage TIMESTAMP NOT NULL,
nouveauMsgAnnonceur CHAR(3) NOT NULL,
nouveauMsgRepondeur CHAR(3) NOT NULL,
INDEX(annonceNO),
INDEX(annonceur),
INDEX(repondeur))
ENGINE INNODB;

ALTER TABLE conversations AUTO_INCREMENT=100000001;