<?php
session_start();
require_once 'fonctions.php';

if (isset($_POST['message']) && isset($_SESSION['pseudo']) ) {

    $expediteur = $_SESSION['pseudo'];
    $receveur = sanitizeString($_POST['receveur']);
    $conversationNO = sanitizeString($_POST['conversationNO']);
    $annonceNO = sanitizeString($_POST['annonceNO']);
    $message = sanitizeString($_POST['message']);

    date_default_timezone_set('Europe/Zurich');
    $dateMessage = date("Y-m-d H:i:s");


    //Module afficher dans le chat les nouveaux messages
    setlocale(LC_TIME, "fr_FR");
    $date = strftime("%d %B %G à %H:%M", strtotime($dateMessage));

    $messageSent = "<div class=\"ligne1\"><div class=\"right\"><div class=\"contenuMessage\">$message</div><div class=\"heureMessage\">Moi, le $date</div></div></div>";
    echo ($messageSent);

    //Module ajout d'un message dans la table messages
    queryMysql("INSERT INTO messages VALUES (NULL, '$conversationNO', '$dateMessage', '$expediteur', '$receveur', '$message')");

    //Module rafraichir l'heure du last message dans la table conversations 
    queryMysql("UPDATE conversations SET lastMessage ='$dateMessage' WHERE conversationNO = '$conversationNO'");

    //Module rafraichir nouveauMsg (si nécessaire) + mailer
    $result = queryMysql("SELECT matiere, annonceur, repondeur, nouveauMsgAnnonceur, nouveauMsgRepondeur FROM conversations WHERE conversationNO = '$conversationNO'");
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $matiere = $row['matiere'];

    $annonceur = $row['annonceur'];
    $repondeur = $row['repondeur'];

    $nouveauMsgAnnonceur = $row['nouveauMsgAnnonceur'];
    $nouveauMsgRepondeur = $row['nouveauMsgRepondeur'];



    //1. Dans la table conversations, checker si l'expediteur (=Session pseudo) est l'ANNONCEUR ou le REPONDEUR
    //2. s'il est l'ANNONCEUR updater à 'oui' la valeur de nouveauMsgRepondeur
    //3. s'il est le REPONDEUR updater à 'oui' la valeur de nouveauMsgAnnonceur
    
    
    if($expediteur == $annonceur){

        if($nouveauMsgRepondeur == 'non') {
            //php mailer : si le destinataire (ici le repondeur) n'avait pas de nouveau message (= 'non'), il va maintenant en avoir un donc envoyer une notif
            //... s'il en avait déjà un ($nouveauMsgRepondeur = autre chose que 'non' (donc 'oui')) : ne rien faire de particulier
            $result = queryMysql("SELECT email FROM utilisateurs WHERE pseudo = '$repondeur'");
            $row = $result->fetch_array(MYSQLI_ASSOC);
            $destinataireEmail = $row['email'];
        
            $to      = $destinataireEmail;
            $subject = 'Nouveau message de la part de '.html_entity_decode($annonceur);
            $mail = 'Nouveau message de de la part de '.html_entity_decode($annonceur).' concernant son annonce dans la discipline '.html_entity_decode($matiere). '.' . "\r\n\r\n" . 'Rends-toi sur www.monduo.ch/mon-compte.php pour lire son message et pour lui répondre.' . "\r\n\r\n" . '© monDUO.ch - 2020';
            $headers = 'From: noreply@monduo.ch' . "\r\n" .
            'X-Mailer: PHP/' . phpversion();
            mail($to, $subject, $mail, $headers);
        }
        
        queryMysql("UPDATE conversations SET nouveauMsgRepondeur ='oui' WHERE conversationNO = '$conversationNO'");
    
    }
    elseif($expediteur == $repondeur){
        
        if($nouveauMsgAnnonceur == 'non') {
            //php mailer : si le destinataire (ici l'annonceur) n'avait pas de nouveau message (= 'non'), il va maintenant en avoir un donc envoyer une notif
            //... s'il en avait déjà un ($nouveauMsgAnnonceur = autre chose que 'non' (donc 'oui')) : ne rien faire de particulier
            $result = queryMysql("SELECT email FROM utilisateurs WHERE pseudo = '$annonceur'");
            $row = $result->fetch_array(MYSQLI_ASSOC);
            $destinataireEmail = $row['email'];

            $to      = $destinataireEmail;
            $subject = 'Nouveau message de la part de '.html_entity_decode($repondeur);
            $mail = 'Nouveau message de la part de '.html_entity_decode($repondeur). ' concernant ton annonce dans la discipline '.html_entity_decode($matiere). '.' . "\r\n\r\n" . 'Rends-toi sur www.monduo.ch/mon-compte.php pour lire son message et pour lui répondre.' . "\r\n\r\n" . '© monDUO.ch - 2020';
            $headers = 'From: noreply@monduo.ch' . "\r\n" .
            'X-Mailer: PHP/' . phpversion();
            mail($to, $subject, $mail, $headers);
        }

        queryMysql("UPDATE conversations SET nouveauMsgAnnonceur ='oui' WHERE conversationNO = '$conversationNO'");
    
    }
    else {
        die('error');
    }
}
?>