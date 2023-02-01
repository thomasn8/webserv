<?php
session_start();
require_once 'fonctions.php';

?>

<!DOCTYPE html>
<html lang="fr">
<head>
   
    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Mon compte - Gérer mes annonces, mon profil et mes contacts - monDUO.ch</title>
    <meta name="description" content="Connecte-toi pour pousuivre tes conversations, modifier tes annonces et administrer ton profil public et privé."/>
    <link rel="canonical" href="https://www.monduo.ch/mon-compte.php/" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Mon compte - Gérer mes annonces, mon profil et mes contacts - monDUO.ch" />
	<meta property="og:description" content="Connecte-toi pour pousuivre tes conversations, modifier tes annonces et administrer ton profil public et privé." />
	<meta property="og:url" content="https://www.monduo.ch/mon-compte.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />

   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
   <link rel="stylesheet" href="mon-compte.css">
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
        <?php
//SI UNE SESSION USER EST ACTIVE
if (isset($_SESSION['pseudo'])) {
    
                //USER CONNECTE + module de déconnexion
    $userPseudo = $_SESSION['pseudo'];
echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter cadre1">
                        <div class="title">Mon compte - $userPseudo</div>
                        <h5 id="userConnected"><span><a href="deconnexion" id="deconnexion">Se déconnecter</a></span></h5>
                        <h5 id="userConnected"><a href="recup-mdp.php" id="deconnexion">Modifier mes informations privées</a></h5>
                        <div class="balises">
                            <a class="balise-monCompte" href="#annonces">Mes annonces</a>
                            <a class="balise-monCompte" href="#contacts">Mes contacts</a>
                            <a class="balise-monCompte" href="#profil">Mon profil</a>
                        </div>
                    </div>
                </div>
            </section>
_END;

                //AFFICHE LES ANNONCES DE L'UTILISATEUR CONNECTE :
echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter">
                        <div id="annonces" class="title">Mes annonces</div>
                        <h5>Sans supression, les annonces restent en ligne pendant 45 jours après publication/prolongation.</h5>
                        <div class="mes-annonces">
_END;
$result = queryMysql("SELECT annonceNO, date, role, domaine, matiere, resume, region, niveau, modalite FROM annonces WHERE pseudo = '$userPseudo' ORDER BY date DESC");

$rows = $result->num_rows;

for ($i = 0 ; $i < $rows ; ++$i) {
    $row = $result->fetch_array(MYSQLI_ASSOC);

    $role = $row['role'];
    if ($role == 1) {
        $role = 'enseigner';
    }
    elseif ($role == 2) {
        $role = 'apprendre';
    }
    elseif ($role == 3) {
        $role = 'partenariat';
    }
    else {
        $role = 'erreur';
    }

    $annonceNO = $row['annonceNO'];
    $domaine = $row['domaine'];
    $matiere = $row['matiere'];
    $resume = apostrophe($row['resume']);
    $pseudo = $row['pseudo'];
    $region = $row['region'];
    $niveau = $row['niveau'];
    $modalite = $row['modalite'];
    $modalite = $row['modalite'];
    if($modalite == 1) {
        $modalite = 'Sans';
    }
    elseif($modalite == 2){
        $modalite = 'Avec';
    }
    elseif($modalite == 3){
        $modalite = 'À voir';
    }
    else{
        $modalite = 'erreur';
    }

    $datepubli = $row['date'];
    setlocale(LC_TIME, "fr_FR");
    $datepubli = strftime("%d %B %G", strtotime($datepubli));

    echo "<div class=\"mon-annonce\"><div class=\"wrap-annonce\"><div class=\"annonce-titre\">$matiere - $role</div>
    <div class=\"champAnnonce\"><div class=\"label\">Domaine :</div><div class=\"champ\">$domaine</div></div> <div class=\"champAnnonce\"><div class=\"label\">Sujet :</div><div class=\"champ\">$resume</div></div> 
    <div class=\"champAnnonce\"><div class=\"label\">Lieu :</div><div class=\"champ\">$region</div></div> <div class=\"champAnnonce\"><div class=\"label\">Ton niveau :</div><div class=\"champ\">$niveau</div></div> 
    <div class=\"champAnnonce\"><div class=\"label\">Rémunération :</div><div class=\"champ\">$modalite</div></div> <div class=\"champAnnonce\"><div class=\"label\">Publiée le :</div><div class=\"champ\">$datepubli</div></div></div>
    <div class=\"champ-button\"><button class=\"mon-annonce-btn1\" id=\"$annonceNO\"><img src=\"images/SVG/delete1.svg\">Supprimer</button>
    <button class=\"mon-annonce-btn2\" id=\"$annonceNO\"><img src=\"images/SVG/refresh1.svg\">Prolonger</button></div></div>";
}
echo <<<_END
                        </div>
                    </div>
                </div>
            </section>
_END;

                //AFFICHE LES CONVERSATIONS DE L'UTILISATEUR CONNECTE :
echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter">
                        <div id="contacts" class="title">Mes contacts</div>
                        <h5>Conversations liées à mes annonces ou aux annonces des utilisateurs qui m'intéressent :</h5>
                        <div class="conversations">
_END;
$result = queryMysql("SELECT * FROM conversations WHERE annonceur = '$userPseudo' OR repondeur = '$userPseudo' ORDER BY lastMessage DESC");

$rows = $result->num_rows;

for ($i = 0 ; $i < $rows ; ++$i) {
    $row = $result->fetch_array(MYSQLI_ASSOC);
    
    $annonceur = $row['annonceur'];
    $repondeur = $row['repondeur'];
    $contact = '';
    if($repondeur == $userPseudo) {
        $contact = $annonceur;
    }
    else {
        $contact = $repondeur;
    }//(à tester si fonctionne dans 100% des cas mais ça a l'air ok)

    $conversationNO = $row['conversationNO'];
    $matiere = $row['matiere'];
    $annonceNO = $row['annonceNO'];

    $nouveauMsgAnnonceur = $row['nouveauMsgAnnonceur'];
    $nouveauMsgRepondeur = $row['nouveauMsgRepondeur'];

    if($repondeur == $userPseudo) {
        if($nouveauMsgRepondeur == 'oui') {
            $newMsg = 'nouveaux messages';
        }
        else{
            $newMsg = '';
        }
    }
    else {
        if($nouveauMsgAnnonceur == 'oui') {
            $newMsg = 'nouveaux messages';
        }
        else{
            $newMsg = '';
        }
    }//(à tester si fonctionne dans 100% des cas mais ça a l'air ok)
    
    echo "<div id=\"$annonceNO\" class=\"conversation\"><a class=\"conv\" href=\"$conversationNO\"><span class=\"contact\">$contact</span><span class=\"matiere\">$matiere</span><span class=\"newMsg\">$newMsg</span></a></div>";
}

echo <<<_END
                        </div>
                    </div>
                </div>
            </section>
_END;

                //PROFIL DE L'UTILISATEUR LES ANNONCES DE L'UTILISATEUR CONNECTE [dans un fieldset en dessous du h5 PROGRAMMER photo + description]
echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter">
                        <div id="profil" class="title">Mon profil $userPseudo</div>
                        <h5 id="userConnected">Informations visibles sur mes annonces : photo de profil et auto-description.</h5>
                        <form method='post' action='mon-compte.php' enctype='multipart/form-data'>
                            <label id="changePicture">
                                <p>Changer ma photo de profil :</p>
                                <input type='file' name='image'>
                            </label>
                            <div id="formProfil">
_END;
$result = queryMysql("SELECT * FROM utilisateursProfiles WHERE pseudo='$userPseudo'");

if (isset($_POST['text']))
{
    $text = sanitizeString($_POST['text']);
    $text = preg_replace('/\s\s+/', ' ', $text);

    if ($result->num_rows)
        queryMysql("UPDATE utilisateursProfiles SET text='$text' where pseudo='$userPseudo'");
    else queryMysql("INSERT INTO utilisateursProfiles VALUES('$userPseudo', '$text')");
}
else
{
    if ($result->num_rows)
    {
    $row  = $result->fetch_array(MYSQLI_ASSOC);
    $text = stripslashes($row['text']);
    }
    else $text = "";
}

$text = stripslashes(preg_replace('/\s\s+/', ' ', $text));

if (isset($_FILES['image']['name']))
{
    $userPseudoDecode = html_entity_decode($userPseudo);
    $saveto = "images/utilisateursProfiles/$userPseudoDecode.jpg";
    move_uploaded_file($_FILES['image']['tmp_name'], $saveto);
    $typeok = TRUE;

    switch($_FILES['image']['type'])
    {
    case "image/gif":   $src = imagecreatefromgif($saveto); break;
    case "image/jpeg":  // Both regular and progressive jpegs
    case "image/pjpeg": $src = imagecreatefromjpeg($saveto); break;
    case "image/png":   $src = imagecreatefrompng($saveto); break;
    default:            $typeok = FALSE; break;
    }

    if ($typeok)
    {
    list($w, $h) = getimagesize($saveto);

    $max = 500;
    $tw  = $w;
    $th  = $h;

    if ($w > $h && $max < $w)
    {
        $th = $max / $w * $h;
        $tw = $max;
    }
    elseif ($h > $w && $max < $h)
    {
        $tw = $max / $h * $w;
        $th = $max;
    }
    elseif ($max < $w)
    {
        $tw = $th = $max;
    }

    $tmp = imagecreatetruecolor($tw, $th);
    imagecopyresampled($tmp, $src, 0, 0, 0, 0, $tw, $th, $w, $h);
    imageconvolution($tmp, array(array(-1, -1, -1),
        array(-1, 16, -1), array(-1, -1, -1)), 8, 0);
    imagejpeg($tmp, $saveto);
    imagedestroy($tmp);
    imagedestroy($src);
    }
}

showProfile($userPseudo);
$text = apostrophe($text);

echo <<<_END
                            <textarea name='text' spellcheck='false' placeholder="Changer mon auto-description ..." maxlength="400">$text</textarea><br>
                        </div>
                        <input id="enregistrer" type='submit' value='Actualiser'>
                        </form>
                    </div>
                </div>
            </section>
_END;
}
//FIN DU MODULE USER CONNECTE



//SI AUCUNE SESSION ACTIVE
            else {
                //AFFICHE LE MODULE DE LOGIN :
echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter">
                        <div class="title">Se connecter</div>
                        <h5>Pas encore de compte monDUO ?<br>Tu peux <a href="creer-compte.php">t'inscrire ici.</a></h5>
                        <fieldset id="publier" class="fieldset-connexion">
                            <legend id="ton-annonce">Identifiants</legend>
                            <form id="connexion" action="loginAndCreateSession" method="post">
                                <div class="identifiants">
                                    <label class="">Nom d'utilisateur ou <span class="ponctuation">adresse e-mail :</span></label>
                                    <input id="identifiant1" type="text" name="identifiant" maxlength="80" autocomplete="on" required />
                                </div>
                                <div class="identifiants">
                                    <label class="">Mot de <span class="ponctuation">passe :</span></label>
                                    <input id="identifiant2" type="password" name="password" maxlength="30" autocomplete="on" required />
                                </div>
                                <h5 id="errorLog"><span id="errorMsg"></span></h5>
                                <h5 id="errorLog"><a class="recup-mdp" href="recup-mdp.php">Identifiants oubliés ?</a></h5>
                                <button id="log-btn" type="submit" name="log-btn">Connexion</button>
                            </form>
                        </fieldset>
                        <div class="avantages">
                            <h5>Sur ton compte tu peux :</h5>
                            <ol>
                                <li><img src="images/SVG/check.svg"><p>Gérer tes annonces</p></li>
                                <li><img src="images/SVG/check.svg"><p>Contacter les autres annonceurs et parler avec eux</p></li>
                                <li><img src="images/SVG/check.svg"><p>Attacher une photo de profil et une description personnelle à tes annonces</p></li>
                                <li><img src="images/SVG/check.svg"><p>Gérer tes données privées</p></li>
                            </ol>
                        </div>
                    </div>
                </div>
            </section>
_END;
            }
        ?>   
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
    <div id="scrollMessage"><img src="images/SVG/arrow.svg"></div>
</div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/scroll.js"></script>
<script src="js/new-window.js"></script>
<?php
if (isset($_SESSION['pseudo'])) {
    echo '<script src="js/deconnexion.js"></script>';
    echo '<script src="js/auto-scroll.js"></script>';
    echo '<script src="js/supprimer-prolonger-monAnnonce.js"></script>';
    echo '<script src="js/chat.js"></script>';
}
else {
    echo '<script src="js/loginAndCreateSessionMonCompte.js"></script>';
}
?> 
</body>
</html>