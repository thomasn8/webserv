CREATE TABLE domaines (
domaineID INT(4) UNSIGNED NOT NULL,
domaine VARCHAR(60) NOT NULL,
INDEX(domaineID),
INDEX(domaine(9)))
ENGINE InnoDB;



INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1000','Artisanats et travaux manuels');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1100','Arts scéniques et littéraires');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1200','Arts visuels');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1300','Bien-être et développement personnel');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1400','Cuisine et gastronomie');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1500','Écoles et formations post-obligatoires');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1600','École obligatoire et cours de soutien');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1700','Informatique et digital');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1800','Langues');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('1900','Musique et instruments');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('2000','Sports');
INSERT INTO domaines(domaineID, domaine) 
    VALUES ('2100','Autres');