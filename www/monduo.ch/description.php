<?php
require_once 'fonctions.php';

$pseudo = sanitizeString($_POST['Pseudo']);

$result = queryMysql("SELECT text FROM utilisateursProfiles WHERE pseudo='$pseudo'");

$rows = $result->num_rows;

for ($i = 0 ; $i < $rows ; ++$i) {
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $text = apostrophe($row['text']);
}

if($text == '')
    $text = 'Rien à dire pour le moment ...';

echo (showProfile($pseudo) . '<div class="description">' . $text . '</div>');

?>