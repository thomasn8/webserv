CREATE TABLE matieres (
matiereID INT(4) UNSIGNED NOT NULL,
matiere VARCHAR(60) NOT NULL,
domaineID INT(4) UNSIGNED NOT NULL,
INDEX(matiereID),
INDEX(matiere(9)),
INDEX(domaineID)) 
ENGINE INNODB;

/* 
Lors de l'insertion des données, attribuer les "matieresID" aux "matiere" en fonction de 
l'ordre alphabétique

+ Attribuer la "matiereID" %99 aux valeurs Autres de chaque domaine
exemple : 1099 Scolaire 1000
exemple : 1199 Sport 1100
*/

INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1001','Agriculture et jardinage','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1002','Broderie, couture et textiles','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1003','Maison et constructions','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1004','Mécanique','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1005','Poterie et sculpture','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1006','Travail du bois','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1007','Travail des métaux','1000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1099','Autres','1000');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1101','Cirque','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1102','Danses','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1103','Éloquence','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1104','Écriture','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1105','Lecture et littérature','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1106','Théâtre','1100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1199','Autres','1100');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1201','Audiovisuel','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1202','Dessin','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1203','Peinture','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1204','Photographie','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1205','Sculpture et arts plastiques','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1206','Vidéo et cinéma','1200');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1299','Autres','1200');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1301','Coaching et thérapie de vie','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1302','Médecine alternative','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1303','Méditation et relaxation','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1304','Nutrition et diététique','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1305','Psychologie','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1306','Yoga','1300');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1399','Autres','1300');

INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1401','Chocolaterie et confiserie','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1402','Cuisine bio','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1403','Cuisine diététique','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1404','Cuisine du monde','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1405','Initiation à la cuisine','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1406','Oenologie','1400');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1499','Autres','1400');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1501','Aide à la rédaction de travaux et relectures','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1502','Aide et cours de soutien','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1503','Allemand scolaire','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1504','Anglais scolaire','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1505','Branches scientifiques','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1506','Comptabilité','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1507','Droit','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1508','Économie','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1509','Études environnementales','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1510','Informatique','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1511','Ingénierie','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1512','Français','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1513','Lettres','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1514','Marketing','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1515','Mathématiques','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1516','Médecine et sciences médicales','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1517','Médias et communication','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1518','Sciences humaines et sociales','1500');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1599','Autres','1500');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1601','Aide, appui et soutien scolaire','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1602','Allemand scolaire','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1603','Anglais scolaire','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1604','Enseignement spécialisé','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1605','Français','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1606','Lecture et écriture','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1607','Mathématiques','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1608','Sciences du vivant','1600');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1699','Autres','1600');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1701','Bureautique','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1702','Design et graphisme','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1703','Développement numérique et programmation','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1704','Initiation aux technologies digitales','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1705','Jeux vidéos et e-sports','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1706','Montage photo','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1707','Montage vidéo','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1708','Réseaux sociaux','1700');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1799','Autres','1700');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1801','Allemand','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1802','Anglais','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1803','Espagnol','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1804','Français','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1805','Italien','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1806','Langues asiatiques','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1807','Échange linguistique','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1808','Tandem','1800');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1899','Autres','1800');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1901','Chant','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1902','Éducation musicale et solfège','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1903','Guitare','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1904','Instruments à vent','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1905','Percussion','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1906','Piano et synthétiseurs','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1907','Violons','1900');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('1999','Autres','1900');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2001','Athlétisme','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2002','Agrès et gymnastique','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2003','Arts martiaux et sports de combat','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2004','Coaching sportif','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2005','Condition physique','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2006','Course à pied','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2007','Danses','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2008','Escalade et via ferrata','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2009','Fitness et crossfit','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2010','Marche et randonnée','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2011','Natation','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2012','Skate, roller et trottinette','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2013','Ski alpin et snowboard','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2014','Ski de rando et raquettes à neige','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2015','Sports collectifs','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2016','Tennis et sports de raquette','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2017','Vélo et cyclisme','2000');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2099','Autres','2000');


INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2101','Babysitting','2100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2102','École de conduite','2100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2103','Garde animalière','2100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2104','Travaux ménagers','2100');
INSERT INTO matieres(matiereID, matiere, domaineID) 
    VALUES ('2199','Autres','2100');