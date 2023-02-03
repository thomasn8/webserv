CREATE TABLE regions (
regionID INT(2) UNSIGNED NOT NULL,
region VARCHAR(40) NOT NULL,
INDEX(regionID),
INDEX(region(2))) 
ENGINE InnoDB;

/*
Insérer les données des villes/régions avec des ID de 1 à 20
1 étant Toute la Suisse, NE PAS L'INSERER dans la base de données MySQL, car déjà paramétrer sur les programmes
2 doit être Par internet

2 Par internet
3 Genève
4 Lausanne
5 Neuchâtel
6 Fribourg
7 Sion
...
*/

INSERT INTO regions(regionID, region) 
    VALUES ('2','Internet');
INSERT INTO regions(regionID, region) 
    VALUES ('3','Berne');
INSERT INTO regions(regionID, region) 
    VALUES ('4','Bienne');
INSERT INTO regions(regionID, region) 
    VALUES ('5','Bulle');
INSERT INTO regions(regionID, region) 
    VALUES ('6','Delémont');
INSERT INTO regions(regionID, region) 
    VALUES ('7','Fribourg');
INSERT INTO regions(regionID, region) 
    VALUES ('8','Genève');
INSERT INTO regions(regionID, region) 
    VALUES ('9','Gland');
INSERT INTO regions(regionID, region) 
    VALUES ('10','La Chaux-de-Fonds');
INSERT INTO regions(regionID, region) 
    VALUES ('11','Lausanne');
INSERT INTO regions(regionID, region) 
    VALUES ('12','Martigny');
INSERT INTO regions(regionID, region) 
    VALUES ('13','Meyrin');
INSERT INTO regions(regionID, region) 
    VALUES ('14','Monthey');
INSERT INTO regions(regionID, region) 
    VALUES ('15','Montreux');
INSERT INTO regions(regionID, region) 
    VALUES ('16','Morges');
INSERT INTO regions(regionID, region) 
    VALUES ('17','Neuchâtel');
INSERT INTO regions(regionID, region) 
    VALUES ('18','Nyon');
INSERT INTO regions(regionID, region) 
    VALUES ('19','Payern');
INSERT INTO regions(regionID, region) 
    VALUES ('20','Sierre');
INSERT INTO regions(regionID, region) 
    VALUES ('21','Sion');
INSERT INTO regions(regionID, region) 
    VALUES ('22','Versoix');
INSERT INTO regions(regionID, region) 
    VALUES ('23','Vevey');
INSERT INTO regions(regionID, region) 
    VALUES ('24','Yverdon');
INSERT INTO regions(regionID, region) 
    VALUES ('25','Zürich');