<?php
session_start();
require_once 'fonctions.php';

$response = '';
$id1 = sanitizeString($_POST['identifiant1']);
$id2 = sanitizeString($_POST['identifiant2']);

$result = queryMySQL("SELECT * FROM utilisateurs WHERE pseudo = '$id1' OR email ='$id1'");

//si l'identifiant entré par l'utilisateur n'est pas reconnu dans la base de données
if ($result->num_rows == 0) {
    
    $response = 'Nom d\'utilisateur ou adresse e-mail inconnu.';
    die($response);
}
else {
    $user = $result->fetch_array(MYSQLI_ASSOC);
    $result->close();

    //si le mot de passe ne match pas avec l'identifiant
    if ((password_verify($id2, $user['password'])) == false) {

        $response = 'Mot de passe invalide.';
        die($response);
    }
    //si l'utilisateur n'a pas clické sur le lien envoyé à son mail et pour valider son compte (verification = 1)
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

        $userPseudo = $user['pseudo'];
        date_default_timezone_set('Europe/Zurich');
        $lastActivity = date("Y-m-d H:i:s");
        queryMysql("UPDATE utilisateurs SET lastActivity = '$lastActivity' WHERE pseudo = '$userPseudo'");

        $response = 'connexion';
        die($response);
    }
    else {
        $response = 'Problème d\'identification. Essaie à nouveau ou <a id="lienvers" href="contact.php">contacte-nous directement ici</a>.';
        die($response);
    }
}

?>