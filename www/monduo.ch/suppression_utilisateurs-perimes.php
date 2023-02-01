<?php
require_once 'fonctions.php';
// Nom de la tâche sur le planificateur : supresseur-utilisateurs

date_default_timezone_set('Europe/Zurich');
$dateNow = date("Y-m-d H:i:s");

//Cron query qui supprime les utilisateurs inactifs depuis plus de + de 365 jours
$result = queryMySQL("SELECT pseudo, email FROM utilisateurs WHERE DATEDIFF('$dateNow', utilisateurs.lastActivity) > 365");
$rows = $result->num_rows;

for ($j = 0 ; $j < $rows ; ++$j) {
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $pseudo = $row['pseudo'];
    $email = $row['email'];

    queryMySQL("DELETE FROM utilisateurs WHERE pseudo = '$pseudo'");
    
    queryMySQL("DELETE FROM utilisateursInformations WHERE email = '$email'");

    queryMySQL("DELETE FROM utilisateursProfiles WHERE pseudo = '$pseudo'");
    
    queryMySQL("DELETE FROM annonces WHERE pseudo = '$pseudo'");

    queryMySQL("DELETE FROM conversations WHERE annonceur = '$pseudo'");
    queryMySQL("DELETE FROM conversations WHERE repondeur = '$pseudo'");

    queryMySQL("DELETE FROM messages WHERE expediteur = '$pseudo'");
    queryMySQL("DELETE FROM messages WHERE receveur = '$pseudo'");
}
?>