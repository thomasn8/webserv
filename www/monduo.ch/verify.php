<?php
require_once 'fonctions.php';
?>

<!DOCTYPE html>
<html lang="fr">
<head>
   
    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Confirmation de la création de mon compte - monDUO.ch</title>
    <meta name="description" content="En ouvrant ce lien, tu viens de confirmer la création de ton compte sur monDUO.ch."/>
    <link rel="canonical" href="https://www.monduo.ch/verify.php/" />
    <meta name="robots" content="noindex,follow" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Confirmation de la création de mon compte - monDUO.ch" />
	<meta property="og:description" content="En ouvrant ce lien, tu viens de confirmer la création de ton compte sur monDUO.ch." />
	<meta property="og:url" content="https://www.monduo.ch/verify.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />

   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
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
            <section class="section-sec">
                <?php
                    if(isset($_GET['email']) && !empty($_GET['email']) AND isset($_GET['verificationCode']) && !empty($_GET['verificationCode'])) {

                        $email = sanitizeString($_GET['email']);
                        $verificationCode = sanitizeString($_GET['verificationCode']);
                    

                        $result = queryMysql("SELECT email, verificationCode FROM utilisateurs WHERE email = '$email' AND verificationCode = '$verificationCode' AND verification = '0'"); 
                        $rows = $result->num_rows;
                            
                        if($rows > 0) {
                            queryMysql("UPDATE utilisateurs SET verification = '1' WHERE email = '$email' AND verificationCode = '$verificationCode' AND verification = '0'");
                            
                            date_default_timezone_set('Europe/Zurich');
                            $dateverification = date("Y-m-d H:i:s");
                            queryMysql("UPDATE utilisateurs SET lastActivity = '$dateverification' WHERE email = '$email'");

                            echo '<div class="title">CONFIRMATION D\'INSCRIPTION</div>';
                            echo '<div class="propuls-text aucentre">Ton compte sur monDUO.ch vient d\'être activé !</div>';
                        }
                        else { 
                            echo '<div class="title">CONFIRMATION D\'INSCRIPTION ÉCHOUÉE</div>';
                            echo '<div class="propuls-text aucentre"><br>URL invalide ou compte déjà activé</div>';
                        }
                    }
                    else {
                        echo '<div class="title">CONFIRMATION D\'INSCRIPTION ÉCHOUÉE</div>';
                        echo '<div class="propuls-text aucentre"><br>URL invalide ou compte déjà activé. Prière de réessayer avec le bon URL...</div>';
                    }
                ?>
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
<script src="js/scroll.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/new-window.js"></script>
</body>
</html>    