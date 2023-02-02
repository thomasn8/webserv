CREATE TABLE niveaux (
niveauID INT(1) UNSIGNED NOT NULL,
niveau VARCHAR(15) NOT NULL,
INDEX(niveauID),
INDEX(niveau(1))) 
ENGINE=InnoDB;


INSERT INTO niveaux(niveauID, niveau) 
    VALUES ('1','Débutant');
INSERT INTO niveaux(niveauID, niveau) 
    VALUES ('2','Moyen');
INSERT INTO niveaux(niveauID, niveau) 
    VALUES ('3','Avancé');
INSERT INTO niveaux(niveauID, niveau) 
    VALUES ('4','Expert');