<?php
session_start();
require_once 'fonctions.php';

$deleteAccountPseudo = $_POST['deleteAccount'];
$userEmail = $_SESSION['email'];


if (!empty($deleteAccountPseudo)) {
    queryMySQL("DELETE FROM utilisateurs WHERE pseudo = '$deleteAccountPseudo'");
    
    queryMySQL("DELETE FROM utilisateursInformations WHERE email = '$userEmail'");

    queryMySQL("DELETE FROM utilisateursProfiles WHERE pseudo = '$deleteAccountPseudo'");
    unlink("images/utilisateursProfiles/$deleteAccountPseudo.jpg");
    
    queryMySQL("DELETE FROM annonces WHERE pseudo = '$deleteAccountPseudo'");

    queryMySQL("DELETE FROM conversations WHERE annonceur = '$deleteAccountPseudo'");
    queryMySQL("DELETE FROM conversations WHERE repondeur = '$deleteAccountPseudo'");

    queryMySQL("DELETE FROM messages WHERE expediteur = '$deleteAccountPseudo'");
    queryMySQL("DELETE FROM messages WHERE receveur = '$deleteAccountPseudo'");

    destroySession();
    exit;
}
?>