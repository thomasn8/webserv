<?php
require_once 'fonctions.php';

//collect the passed id
$id = $_GET['domaineID'];


//Sélectionne toutes les matières (sans les matieres Autres %99) => au chargement ou au changement
if($_GET['domaineID'] == 'allDomaines' ) {
    $result = queryMysql("SELECT matiereID,matiere FROM matieres WHERE matiereID NOT LIKE '%99' ORDER BY matiere");

    $rows = $result->num_rows;

    for ($k = 0 ; $k < $rows ; ++$k) {
        $row = $result->fetch_array(MYSQLI_ASSOC);

        $matiereID = $row['matiereID'];
        $matiere = $row['matiere'];

        echo "<option value='$matiereID'>$matiere</option>";
    }
}

//Sélectionne les matières en fonction du domaine sélectionné (y compris les matieres Autres %99) => au changement
else {
    $result = queryMysql("SELECT matiereID,matiere FROM matieres WHERE domaineID = '$id' ORDER BY matiereID");

    $rows = $result->num_rows;

    for ($j = 0 ; $j < $rows ; ++$j) {
        $row = $result->fetch_array(MYSQLI_ASSOC);

        $matiereID = $row['matiereID'];
        $matiere = $row['matiere'];

        echo "<option value='$matiereID'>$matiere</option>";
    }
}
?>