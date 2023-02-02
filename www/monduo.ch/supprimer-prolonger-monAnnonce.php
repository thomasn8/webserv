<?php
require_once 'fonctions.php';

$supprimer = $prolonger = '';
$supprimer = $_POST['annonceDelete'];
$prolonger = $_POST['annonceUpdate'];

date_default_timezone_set('Europe/Zurich');
$dateProlong = date("Y-m-d H:i:s");


if (!empty($supprimer)) {
    queryMySQL("DELETE FROM annonces WHERE annonceNO = '$supprimer'");

    exit;
}
if (!empty($prolonger)) {
    queryMySQL("UPDATE annonces SET date='$dateProlong' WHERE annonceNO = '$prolonger'");

    exit;
}
?>