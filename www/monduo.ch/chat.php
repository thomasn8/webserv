<?php
session_start();
require_once 'fonctions.php';

$conversationNO = $_POST['conversationNO'];
$contact = $_POST['contact'];
$userConnected = $_SESSION['pseudo'];

$limitMsg = 20;
$limit = '';
$limit = $_POST['limit'];

if($limit == 'limit20') {
    $limitMsg = 20;
}
elseif($limit == 'limit40') {
    $limitMsg = 40;
}
elseif($limit == 'limit100') {
    $limitMsg = 100;
}


//Module affichage du chat dans la div#login
$result = queryMysql("SELECT * FROM messages WHERE conversationNO = '$conversationNO' ORDER BY dateMessage DESC LIMIT $limitMsg");
$rows = $result->num_rows;

for ($i = 0 ; $i < $rows ; ++$i) {
    $row = $result->fetch_array(MYSQLI_ASSOC);
    
    $dateMessage = $row['dateMessage'];
    setlocale(LC_TIME, "fr_FR");
    $dateMessage = strftime("%d %B %G à %H:%M", strtotime($dateMessage));
    //Eventuellement créer une formule qui calcule la diff de temps entre maintenant - dateMessage et qui remplace la date (%d %B %G) par Aujourd'hui / Hier si ça sort de la date du jour ou de celle d'hier 
    //... pour ça : https://www.it-swarm.dev/fr/php/php-comment-verifier-si-une-date-est-aujourdhui-hier-ou-demain/1047702410/

    $expediteur = $row['expediteur'];
    $receveur = $row['receveur'];
    $message = apostrophe($row['message']);

    if($expediteur == $userConnected) {
        echo "<div class=\"ligne1\"><div class=\"right\"><div class=\"contenuMessage\">$message</div><div class=\"heureMessage\">Moi, le $dateMessage</div></div></div>";
    }
    else {
        echo "<div class=\"ligne2\"><div class=\"left\"><div class=\"contenuMessage\">$message</div><div class=\"heureMessage\">$contact, le $dateMessage</div></div></div>";
    }
    
}


//Module effaçage de la notification "nouveaux messages"
$result = queryMysql("SELECT annonceur, repondeur FROM conversations WHERE conversationNO = '$conversationNO'");
$row = $result->fetch_array(MYSQLI_ASSOC);

$annonceur = $row['annonceur'];
$repondeur = $row['repondeur'];

if($userConnected == $annonceur){
    queryMysql("UPDATE conversations SET nouveauMsgAnnonceur ='non' WHERE conversationNO = '$conversationNO'");
}
elseif($userConnected == $repondeur){
    queryMysql("UPDATE conversations SET nouveauMsgRepondeur ='non' WHERE conversationNO = '$conversationNO'");
}
else {
    die('error');
}

?>