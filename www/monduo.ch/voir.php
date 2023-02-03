<?php
session_start();
require_once 'fonctions.php';

if (isset($_POST['sendMessage']) && isset($_SESSION['pseudo']) ) {

    $annonceur = trim(sanitizeString($_POST['annonceur'])); //obliger de mettre le trim() sinon rajoute un espace à cause du html/css (:after)
    $annonceNO = sanitizeString($_POST['annonceNO']);
    $matiere = sanitizeString($_POST['matiere']);
    $message = sanitizeString($_POST['message']);

    date_default_timezone_set('Europe/Zurich');
    $dateDebut = date("Y-m-d H:i:s");
    $lastMessage = date("Y-m-d H:i:s");

    $repondeur = $_SESSION['pseudo'];

    $result = queryMysql("SELECT * FROM conversations WHERE annonceNO='$annonceNO' AND repondeur='$repondeur'");

    if ($result->num_rows)
        echo ("<div id=\"login\"><span id=\"close\"></span>Annonce déjà contactée.<br><br>Tu peux retrouver la conversation liée à cette annonce sur ton espace <a id=\"lienvers\" href=\"mon-compte.php\">Mon compte</a>.</div><script src=\"js/messageSignin.js\"></script></body></html>");
    else {
        //création d'une nouvelle conversation dans la table conversations
        queryMysql("INSERT INTO conversations VALUES (NULL, '$annonceNO', '$matiere', '$dateDebut', '$annonceur', '$repondeur', '$lastMessage', 'oui', 'non')");

        //Mailer pour avertir d'un nouveau message
        $result = queryMysql("SELECT email FROM utilisateurs WHERE pseudo = '$annonceur'");
        $rows = $result->fetch_array(MYSQLI_ASSOC);
        $result->close();
        $annonceurEmail = $rows['email'];

        $to      = $annonceurEmail;
        $subject = 'Nouveau message de ' .html_entity_decode($repondeur);
        $mail = 'Nouveau contact à propos de ton annonce dans la discipline '.html_entity_decode($matiere).' : ' .$repondeur. ' t\'a envoyé un message.' . "\r\n\r\n" . 'Rends-toi sur www.monduo.ch/mon-compte.php pour lire son message et lui répondre.' . "\r\n\r\n" .'© monDUO.ch - 2020';
        $headers = 'From: noreply@monduo.ch' . "\r\n" .
        'X-Mailer: PHP/' . phpversion();
        mail($to, $subject, $mail, $headers);

        //Inscription du message dans la table messages
        $result = queryMysql("SELECT conversationNO, annonceur, repondeur FROM conversations WHERE annonceNO = '$annonceNO' AND annonceur = '$annonceur' AND repondeur = '$repondeur'");
        $conv = $result->fetch_array(MYSQLI_ASSOC);
        $result->close();

        $conversationNO = $conv['conversationNO'];
        $annonceur = $conv['annonceur'];
        $repondeur = $conv['repondeur'];
        
        queryMysql("INSERT INTO messages VALUES (NULL, '$conversationNO', '$dateDebut', '$repondeur', '$annonceur', '$message')");

        echo ("<div id=\"login\"><span id=\"close\"></span>Message envoyé.<br><br>Poursuis cette conversation en tout temps <span class=\"ponctuation\">sur ton espace <a id=\"lienvers\" href=\"mon-compte.php\">Mon compte</a>.</span><br><br>Tu recevras une notification par email si $annonceur te répond.</div><script src=\"js/messageSignin.js\"></script></body></html>");
    }
} 

?>

<!DOCTYPE html>
<html lang="fr">
<head>

    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Voir les annonces et contacter les personnes qui t'intéressent - monDUO.ch</title>
    <meta name="description" content="Chercher parmi les annonces le profil qui t'intéresse pour former un DUO et lancer une conversation."/>
    <link rel="canonical" href="https://www.monduo.ch/voir.php/" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Voir les annonces et contacter les personnes qui t'intéressent - monDUO.ch" />
	<meta property="og:description" content="Chercher parmi les annonces le profil qui t'intéresse pour former un DUO et lancer une conversation." />
	<meta property="og:url" content="https://www.monduo.ch/voir.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />
   
   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
   <link rel="stylesheet" href="selectbox.css">
   <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
</head>
<body>
<div class="wrapper">
    <header class="topbar">
       <div class="topbar-logo">
            <div class="logo-cadre">
                <div id="scale"><a href="index.html">monDUO<span id="ch">.ch</span></a></div>
            </div>
        </div>
        <nav class="topbar-nav">
           <a class="btn" href="index.html">Page d'accueil</a>
           <a class="btn" href="publier.php">Publier une annonce</a>
           <a class="btn active" href="voir.php">Voir les annonces</a>
           <a class="btn" href="mon-compte.php">Mon compte</a>
        </nav>
    </header>
    <div class="container">
        <main class="main-sec">

            <section id="section-tableau" class="section-sec">
                <div class="title">Voir les annonces</div>
                <h5>Pour répondre à une annonce, tu dois avoir un compte monDUO.<br>Tu peux <a href="creer-compte.php">t'inscrire ici.</a></h5>
                <div class="tableau">
                    <form>
                        <fieldset id="publier">
                            <legend id="ton-annonce">Rechercher</legend>
                            <label class="identifiants-select retrait2">Prof.e / Élève / Partenaire ?
                                <select class="filtres" id="role" name="role">
                                    <option id="default-filtre" value="0" selected="selected" disabled>Toutes les annonces</option>
                                    <option value="1">Prof.e.s</option>
                                    <option value="2">Élèves</option>
                                    <option value="3">Partenaires</option>
                                </select>
                            </label>

                            <label class="identifiants-select retrait2">Domaine :
                            <select class="filtres" id="domaineID" name="domaineID">
                                <option value="allDomaines" selected="selected">Tous</option>
                                <?php 
                                $result = queryMysql("SELECT domaineID,domaine FROM domaines ORDER BY domaine");

                                $rows = $result->num_rows;

                                for ($j = 0 ; $j < $rows ; ++$j) {
                                    $row = $result->fetch_array(MYSQLI_ASSOC);

                                    $domaineID = $row['domaineID'];
                                    $domaine = $row['domaine'];

                                    echo "<option value='$domaineID'>$domaine</option>";
                                }
                                ?></select>
                            </label>

                            <label class="identifiants-select retrait2">Discipline :
                            <select class="filtres" id="matiereID" name="matiereID">
                                <option value="allMatiere" selected="selected">Toutes</option>
                            </select>
                            </label>

                            <label class="identifiants-select retrait2 noWrap">Où ? <span class="ponctuation">(internet / régions)</span>
                            <select class="filtres" id="regionID" name="regionID">
                                <option value="allRegion" selected="selected">Toute la Suisse</option>
                                <?php 
                                $result = queryMysql("SELECT regionID,region FROM regions ORDER BY regionID");

                                $rows = $result->num_rows;

                                for ($k = 0 ; $k < $rows ; ++$k) {
                                    $row = $result->fetch_array(MYSQLI_ASSOC);

                                    $regionID = $row['regionID'];
                                    $region = $row['region'];

                                    echo "<option value='$regionID'>$region</option>";
                                }
                                ?></select>
                            </label>
                        </fieldset>
                    </form>

                    <table>
                        <tr id="th">
                            <th class="left skill"><img src="images/SVG/presentation/skill.svg" title="Discipline" alt="Discipline"></th><th class="left resume"><img src="images/SVG/presentation/settings.svg" title="Au sujet de..." alt="Au sujet de..."></th><th class="center"><img src="images/SVG/presentation/person.svg" title="Annonceur" alt="Annonceur"></th><th class="center"><img src="images/SVG/presentation/map.svg"  title="Dans la région de..." alt="Dans la région de..."></th><th class="right level"><img src="images/SVG/presentation/level.svg"  title="Son niveau" alt="Son niveau"></th><th class="right price"><img src="images/SVG/presentation/price.svg" title="Avec rémunération ?" alt="Avec rémunération ?"></th><th></th>
                        </tr>
                        <?php
                            if(isset($_GET['id'])) {
                                $id = sanitizeString($_GET['id']);

                                $result = queryMysql("SELECT annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces WHERE domaineID = '$id' OR matiereID = '$id' ORDER BY date DESC LIMIT 20");

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
                            }
                            else {
                                $result = queryMysql("SELECT annonceNO, matiere, resume, pseudo, region, niveau, modalite FROM annonces ORDER BY date DESC LIMIT 20");

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
                            }
                        ?>
                    </table>

                    <label id="limit">
                        <h5>voir</h5>
                        <label class="lab-lim">
                            <p class="lim-nbr">20</p>
                            <input class="filtres filtre-limit" type="radio" name="limit" value="20" checked="checked">
                        </label>
                        <label class="lab-lim">
                            <p class="lim-nbr">50</p>
                            <input class="filtres filtre-limit" type="radio" name="limit" value="50">
                        </label>
                        <label class="lab-lim">
                            <p class="lim-nbr">100</p>
                            <input class="filtres filtre-limit" type="radio" name="limit" value="100">
                        </label>
                        <h5>annonces</h5>
                    </label>
                </div>   
            </section>
        </main>
    </div>
    <footer class="bottom-nav">
        <div class="footer-nav">
            <a class="btn" href="conditions.html">Conditions d'utilisation</a>
            <a class="btn" href="a-propos.html">À propos</a>
            <a class="btn" href="contact.php">Nous contacter</a>
        </div>
        <div class="credits">
            <div class="fb-tw">
                <a class= "fb new-fb" href="https://www.facebook.com/Monduoch-653782461899658/"><img src="images/SVG/fb.svg"></a>
                <a class= "tw new-lk" href="https://www.linkedin.com/company/monduo-ch/"><img src="images/SVG/lk.svg"></a>
            </div>
            <div class="credit">© monDUO.ch 2020</div>
            <div class="developer">mis en place par <a id="newWindow" href="https://caricactus.ch">caricactus.ch</a></div> 
        </div>
    </footer>
</div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/new-window.js"></script>
<script src="js/selectMatiereVoir.js"></script>
<script src="js/getFiltres.js"></script>
<script src="js/profile-hover.js"></script>

<?php
// module de réponse aux annonces
if (isset($_SESSION['pseudo'])) {
    echo ('<script src="js/filtresSESSION.js"></script>');
    echo ('<script src="js/annonce_contact.js"></script>');
}

// module de connexion
else {
    echo ('<script src="js/filtresNOSESSION.js"></script>');
    echo ('<script src="js/connectToContact.js"></script>');
}
?>
<!--<div id="scrollMessage"><img src="images/SVG/arrow.svg"></div>
<script src="js/scroll.js"></script>-->
</body>
</html>