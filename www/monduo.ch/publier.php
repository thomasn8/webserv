<?php
session_start();
require_once 'fonctions.php';


if ( isset($_POST['publish-btn']) && isset($_SESSION['pseudo']) ) {

    date_default_timezone_set('Europe/Zurich');
    $date = date("Y-m-d H:i:s");

    $role = sanitizeString($_POST['role']);
    $modalite = sanitizeString($_POST['modalite']);

    $domaineID = sanitizeString($_POST['domaineID']);
        $result = queryMysql("SELECT domaine FROM domaines WHERE domaineID = '$domaineID'");
        $row = $result->fetch_array(MYSQLI_ASSOC);
    $domaine = $row['domaine'];

    $matiereID = sanitizeString($_POST['matiereID']);
        $result = queryMysql("SELECT matiere FROM matieres WHERE matiereID = '$matiereID'");
        $row = $result->fetch_array(MYSQLI_ASSOC);
    $matiere = $row['matiere'];

    $regionID = sanitizeString($_POST['regionID']);
        if($regionID == 1){
            $region = 'Toute la Suisse';
        }
        else {
            $result = queryMysql("SELECT region FROM regions WHERE regionID = '$regionID'");
            $row = $result->fetch_array(MYSQLI_ASSOC);
            $region = $row['region'];
        }

    $niveauID = sanitizeString($_POST['niveau']);
        $result = queryMysql("SELECT niveau FROM niveaux WHERE niveauID = '$niveauID'");
        $row = $result->fetch_array(MYSQLI_ASSOC);
    $niveau = $row['niveau'];

    $resume = sanitizeString($_POST['resume']);

    $pseudo = $_SESSION['pseudo'];

    //CHECK COMBIEN L'UTILISATEUR POSSEDE D'ANNONCE DANS LA DB ET FIXE UN MAX A 5
    $result = queryMysql("SELECT count(pseudo) AS total FROM annonces WHERE pseudo = '$pseudo'");
    $rows = $result->fetch_array(MYSQLI_ASSOC);
    $totalAnnonces = $rows['total'];
    if ($totalAnnonces <= 4) {
        queryMysql("INSERT INTO annonces VALUES (NULL, '$date', '$pseudo', '$role', '$domaine', '$domaineID', '$matiere', '$matiereID', '$resume', '$region', '$regionID', '$niveau', '$modalite')");
        echo ('<div id="login"><span id="close"></span>Ton annonce vient d\'être publiée. Tu peux la consulter et la modifier sur ton espace <a id="lienvers" href="mon-compte.php">Mon compte</a>.<br><br>Tu recevras une notification par email si quelqu\'un te répond.</div><script src="js/messageSignin.js"></script></body></html>');
    }
    else {
        echo ('<div id="login"><span id="close"></span>5 annonces maximum par utilisateur. Rends-toi sur ton espace <a id="lienvers" href="mon-compte.php">mon compte</a> pour gérer tes annonces.</div><script src="js/messageSignin.js"></script></body></html>');
    }
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>

    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Publier une annonce pour chercher un DUO - monDUO.ch</title>
    <meta name="description" content="En publiant une annonce, tu peux mettre en avant tes disponibilités en tant que prof, élève ou partenaire dans une discipline."/>
    <link rel="canonical" href="https://www.monduo.ch/publier.php/" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Publier une annonce pour chercher un DUO - monDUO.ch" />
	<meta property="og:description" content="En publiant une annonce, tu peux mettre en avant tes disponibilités en tant que prof, élève ou partenaire dans une discipline." />
	<meta property="og:url" content="https://www.monduo.ch/publier.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />
   
   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
   <link rel="stylesheet" href="selectbox.css">
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
           <a class="btn" href="voir.php">Voir les annonces</a>
           <a class="btn" href="mon-compte.php">Mon compte</a>
        </nav>
    </header>
    <div class="container">
        <main class="main-sec">
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter"><form id="publish" action="publier.php" method="post">
                        <div class="title">Publier une annonce</div>
                        <h5>Pour publier une annonce, tu dois avoir un compte monDUO.<br>Tu peux <a href="creer-compte.php">t'inscrire ici.</a></h5>

                        <fieldset id="publier">
                        <legend id="ton-annonce">Ton annonce</legend>
                            <div class="identifiants radiobuttons">
                                <label class="first-label">Ton <span class="ponctuation">rôle ?</span></label>
                                <div class="checkboxes">
                                    <label class="radio-label">Prof.e
                                        <input id="" class="form1 radio role" type="radio" name="role" value="1" checked="checked">
                                        <span class="checked"></span>
                                    </label>

                                    <label class="radio-label">Élève
                                        <input id="" class="form1 radio role" type="radio" name="role" value="2">
                                        <span class="checked"></span>
                                    </label>

                                    <label class="radio-label">Partenaire
                                        <input id="" class="form1 radio role" type="radio" name="role" value="3">
                                        <span class="checked"></span>
                                    </label>
                                </div>
                            </div>


                            <div class="identifiants identifiants-select">
                                <label class=""><span class="ponctuation">Domaine :</span></label>
                                <select id="domaineID" name="domaineID" class="form1" required>
                                    <option value="" selected="selected" disabled></option>
                                    <?php
                                        $result = queryMysql("SELECT domaineID,domaine FROM domaines ORDER BY domaine");

                                        $rows = $result->num_rows;

                                        for ($j = 0 ; $j < $rows ; ++$j) {
                                            $row = $result->fetch_array(MYSQLI_ASSOC);

                                            $domaineID = $row['domaineID'];
                                            $domaine = $row['domaine'];

                                            echo "<option value='$domaineID'>$domaine</option>";
                                        }
                                    ?>
                                </select>
                            </div>


                            <div class="identifiants identifiants-select">
                                <label class=""><span class="ponctuation">Discipline :</span></label>
                                <select id="matiereID" name="matiereID" class="form1" required>
                                </select>
                            </div>


                            <div id="correction-padding" class="identifiants">
                                <label class="">Ce qui t'intéresse <span class="ponctuation">(en bref) :</span></label>
                                <input id="resume" type="text" name="resume" maxlength="80" autocomplete='on' class="form1" required />
                            </div>


                            <div class="identifiants identifiants-select">
                                <label class=""><span class="ponctuation">Où ?</span> <span class="ponctuation">(internet/régions) :</span></label>
                                <select id="regionID" name="regionID" class="form1" required>
                                    <option value="" selected="selected" disabled></option>
                                    <!--<option value="1">Toute la Suisse</option>-->
                                        <?php
                                            $result = queryMysql("SELECT regionID,region FROM regions ORDER BY regionID");

                                            $rows = $result->num_rows;

                                            for ($k = 0 ; $k < $rows ; ++$k) {
                                                $row = $result->fetch_array(MYSQLI_ASSOC);

                                                $regionID = $row['regionID'];
                                                $region = $row['region'];

                                                echo "<option value='$regionID'>$region</option>";
                                            }
                                        ?>
                                </select>
                            </div>

                            <div class="identifiants identifiants-select">
                                <label class="">Ton niveau dans cette <span class="ponctuation">discipline :</span></label>
                                <select id="niveau" name="niveau" class="form1" required>
                                    <option value="" selected="selected" disabled></option>
                                    <?php
                                        $result = queryMysql("SELECT niveauID,niveau FROM niveaux ORDER BY niveauID");

                                        $rows = $result->num_rows;

                                        for ($m = 0 ; $m < $rows ; ++$m) {
                                            $row = $result->fetch_array(MYSQLI_ASSOC);

                                            $niveauID = $row['niveauID'];
                                            $niveau = $row['niveau'];

                                            echo "<option value='$niveauID'>$niveau</option>";
                                        }
                                    ?>
                                </select>
                            </div>


                            <div class="identifiants radiobuttons">
                                <label class="first-label"><span class="ponctuation">Rémunération ?</span></label>
                                <div class="checkboxes">
                                    <label class="radio-label">Sans
                                        <input id="" class="form1 radio modalite" type="radio" name="modalite" value="1" checked="checked">
                                        <span class="checked"></span>
                                    </label>

                                    <label class="radio-label">Avec
                                        <input id="" class="form1 radio modalite" type="radio" name="modalite" value="2">
                                        <span class="checked"></span>
                                    </label>

                                    <label class="radio-label">On verra
                                        <input id="" class="form1 radio modalite" type="radio" name="modalite" value="3">
                                        <span class="checked"></span>
                                    </label>
                                </div>
                            </div>


                            <button type="submit" name="publish-btn">Publier</button>
                        </fieldset>
                    </form></div>
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
<div id="scrollMessage"><img src="images/SVG/arrow.svg"></div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/scroll.js"></script>
<script src="js/new-window.js"></script>
<script src="js/selectMatierePublier.js"></script>
<?php
    if (!isset($_SESSION['pseudo'])) {   
        echo ('<script src="js/preventSubmitForLogin.js"></script>');
    }
?>
</body>
</html>