<?php
session_start();
require_once 'fonctions.php';

$response = '';
$id1 = sanitizeString($_POST['identifiant1']);
$id2 = sanitizeString($_POST['identifiant2']);

date_default_timezone_set('Europe/Zurich');
$date = date("Y-m-d H:i:s");

$role = sanitizeString($_POST['role']);
$modalite = sanitizeString($_POST['modalite']);

$domaineID = sanitizeString($_POST['domaineID']);
$matiereID = sanitizeString($_POST['matiereID']);
$resume = sanitizeString($_POST['resume']);
$regionID = sanitizeString($_POST['regionID']);
$niveauID = sanitizeString($_POST['niveauID']);

$result = queryMysql("SELECT domaine FROM domaines WHERE domaineID = '$domaineID'");
$row = $result->fetch_array(MYSQLI_ASSOC);
$domaine = $row['domaine'];

$result = queryMysql("SELECT matiere FROM matieres WHERE matiereID = '$matiereID'");
$row = $result->fetch_array(MYSQLI_ASSOC);
$matiere = $row['matiere'];

if($regionID == 1){
    $region = 'Toute la Suisse';
}
else {
    $result = queryMysql("SELECT region FROM regions WHERE regionID = '$regionID'");
    $row = $result->fetch_array(MYSQLI_ASSOC);
    $region = $row['region'];
}

$result = queryMysql("SELECT niveau FROM niveaux WHERE niveauID = '$niveauID'");
$row = $result->fetch_array(MYSQLI_ASSOC);
$niveau = $row['niveau'];




$result = queryMySQL("SELECT * FROM utilisateurs WHERE pseudo = '$id1' OR email ='$id1'");

//si l'identifiant entré par l'utilisateur n'est pas reconnu dans la base de données
if ($result->num_rows == 0) {
    
    $response = 'Nom d\'utilisateur ou adresse e-mail inconnu.';
    die($response);
}
else {
    $user = $result->fetch_array(MYSQLI_ASSOC);
    $result->close();

    //si le mot de passe match avec l'identifiant
    if ((password_verify($id2, $user['password'])) == false) {

        $response = 'Mot de passe invalide.';
        die($response);
    }
    //si l'utilisateur n'a pas clické sur le lien envoyé à son mail pour valider son compte (verification = 1)
    elseif ($user['verification'] !== '1') {

        $response = 'Tu n\'as pas encore validé ton compte grâce au lien qu\'on t\'a envoyé à ton adresse email.';
        die($response);
    }
    //si aucune erreur
    elseif ((password_verify($id2, $user['password'])) && ($user['verification'] == '1')) {

        $_SESSION['pseudo'] = $user['pseudo'];
        $_SESSION['email'] = $user['email'];
        $_SESSION['verification'] = $user['verification'];
        $_SESSION['LAST_ACTIVITY'] = time();

        $pseudo = $_SESSION['pseudo'];
        date_default_timezone_set('Europe/Zurich');
        $lastActivity = date("Y-m-d H:i:s");
        queryMysql("UPDATE utilisateurs SET lastActivity = '$lastActivity' WHERE pseudo = '$pseudo'");

        //CHECK COMBIEN L'UTILISATEUR POSSEDE D'ANNONCE DANS LA DB ET FIXE UN MAX A 5
        $result = queryMysql("SELECT count(pseudo) AS total FROM annonces WHERE pseudo = '$pseudo'");
        $rows = $result->fetch_array(MYSQLI_ASSOC);
        $totalAnnonces = $rows['total'];
        if ($totalAnnonces <= 4) {
            
            queryMysql("INSERT INTO annonces VALUES (NULL, '$date', '$pseudo', '$role', '$domaine', '$domaineID', '$matiere', '$matiereID', '$resume', '$region', '$regionID', '$niveau', '$modalite')");
            
            $response = 'Publication de l\'annonce...';
            die($response);
        }
        else {
            $response = 'Nombre maximum d\'annonce atteint';
            die($response);
        }
    }
    else {
        $response = 'Problème d\'identification. Essaie à nouveau ou <a id="lienvers" href="contact.php">contacte-nous directement ici</a>.';
        die($response);
    }
}

?>