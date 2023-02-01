<?php
require_once 'fonctions.php';
// Nom de la tâche sur le planificateur : supresseur-annonces

date_default_timezone_set('Europe/Zurich');
$dateNow = date("Y-m-d H:i:s");

/* Suspendre la supression des annonces

//Cron queriy qui supprime les annonces de + de 45 jours
queryMySQL("DELETE FROM annonces WHERE DATEDIFF('$dateNow', annonces.date) > 45");

//Cron queries qui prolongent les annonces des comptes fictifs
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 30 DAY) WHERE annonceNo = '22'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 31 DAY) WHERE annonceNo = '13'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 32 DAY) WHERE annonceNo = '12'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 33 DAY) WHERE annonceNo = '11'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 34 DAY) WHERE annonceNo = '10'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 35 DAY) WHERE annonceNo = '9'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 36 DAY) WHERE annonceNo = '8'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 37 DAY) WHERE annonceNo = '7'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 38 DAY) WHERE annonceNo = '6'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 39 DAY) WHERE annonceNo = '4'");
queryMySQL("UPDATE annonces SET date = DATE_SUB(NOW(), INTERVAL 40 DAY) WHERE annonceNo = '3'");
*/

?>