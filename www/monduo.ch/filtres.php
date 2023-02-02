<?php
require_once 'fonctions.php';

function selectAndReturnData($result) {
    $rows = $result->num_rows;

    for ($i = 0 ; $i < $rows ; ++$i) {
        $row = $result->fetch_array(MYSQLI_ASSOC);

        $annonceNO = $row['annonceNO'];
        $matiere = $row['matiere'];
        $resume = apostrophe($row['resume']);
        $pseudo = $row['pseudo'];
        $region = $row['region'];
        $niveau = $row['niveau'];
        $modalite = $row['modalite'];
        if($modalite == 1) {
            $modalite = '<span class="gratis"> non </span>';
        }
        elseif($modalite == 2){
            $modalite = '<span class="remun"> oui </span>';
        }
        elseif($modalite == 3){
            $modalite = '<span class="a-voir">à voir</span>';
        }
        else{
            $modalite = 'erreur';
        }

        echo "<tr><td class=\"discipline\">$matiere</td><td>$resume</td><td class=\"annonceur\"><a href=\"#\" id=\"$pseudo\" class=\"profile-button\">$pseudo </a></td><td>$region</td><td>$niveau</td><td>$modalite</td><td><button class=\"contactButton\" id=\"$annonceNO\"><img src=\"images/SVG/presentation/contact.svg\"></button></td></tr>";
    }
    exit;
};

//obtient l'ID de l'option sélectionnée
$role = $_GET['role'];
$id1 = $_GET['domaineID'];
$id2 = $_GET['matiereID'];
$id3 = $_GET['regionID'];
$id4 = $_GET['limit'];

if ($role == 0) {
    if (($id1 == 'allDomaines') && ($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id2 == 'allMatiere')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id1 == 'allDomaines') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE domaineID = '$id1' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id3 == 'allRegion') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE domaineID = '$id1' AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id2 == 'allMatiere') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE domaineID = '$id1' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    else {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE domaineID = '$id1' AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
}
elseif ($role == 1) {
    if (($id1 == 'allDomaines') && ($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id2 == 'allMatiere')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id1 == 'allDomaines') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND domaineID = '$id1' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id3 == 'allRegion') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND domaineID = '$id1' AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id2 == 'allMatiere') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND domaineID = '$id1' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    else {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 1 AND domaineID = '$id1' AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
}
elseif ($role == 2) {
    if (($id1 == 'allDomaines') && ($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id2 == 'allMatiere')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id1 == 'allDomaines') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND domaineID = '$id1' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id3 == 'allRegion') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND domaineID = '$id1' AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id2 == 'allMatiere') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND domaineID = '$id1' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    else {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 2 AND domaineID = '$id1' AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
}
elseif ($role == 3) {
    if (($id1 == 'allDomaines') && ($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id2 == 'allMatiere')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id1 == 'allDomaines') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id1 == 'allDomaines') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif (($id2 == 'allMatiere') && ($id3 == 'allRegion')) {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND domaineID = '$id1' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id3 == 'allRegion') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND domaineID = '$id1' AND matiereID = '$id2' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    elseif ($id2 == 'allMatiere') {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND domaineID = '$id1' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
    else {
        $result = queryMysql("SELECT role annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE role = 3 AND domaineID = '$id1' AND matiereID = '$id2' AND regionID = '$id3' ORDER BY date DESC LIMIT $id4");
        selectAndReturnData($result);
    }
}
else die('problème');
?>