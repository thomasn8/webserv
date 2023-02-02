<?php
session_start();
require_once 'fonctions.php';
destroySession();
?>


<!DOCTYPE html>
<html lang="fr">
<head>

    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Créer un compte rapidement et gratuitement sur monDUO.ch</title>
    <meta name="description" content="Grâce à ton compte tu pourras publier des annonces et contacter les autres utilisateurs de monDUO.ch."/>
    <link rel="canonical" href="https://www.monduo.ch/creer-compte.php/" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Créer un compte rapidement et gratuitement sur monDUO.ch" />
	<meta property="og:description" content="Grâce à ton compte tu pourras publier des annonces et contacter les autres utilisateurs de monDUO.ch." />
	<meta property="og:url" content="https://www.monduo.ch/creer-compte.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />

   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
   <script src="https://www.google.com/recaptcha/api.js?render=6Ld_wsEZAAAAAP3TAMsVjukyERk_QMuyL2e6Kkpp"></script>
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
           <a href="index.html">Page d'accueil</a>
           <a href="publier.php">Publier une annonce</a>
           <a href="voir.php">Voir les annonces</a>
           <a href="mon-compte.php">Mon compte</a>
        </nav>
    </header>
    <div class="container">
        <main class="main-sec">
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="title"><balise id="ancre">Créer un compte</balise></div>
                    <h5 class="lire-conditions">Avant de créer ton compte, veille à lire attentivement les <a href="conditions.html">Conditions d'utilisation</a> de monDUO.ch.</h5>
                    <fieldset id="publier">
                        <legend id="ton-annonce">Ton compte</legend>
                        <div class="creer-compte">
                        <form id="newuser" action="creer-compte.php" method="post">
                            <h4 id="identifiants" class="h4-creer">Identifiants :</h4>
                            <div class="identifiants">
                                <label class="">Adresse <span class="ponctuation">e-mail :</span></label>
                                <input id="email" type="email" name="email" maxlength="80" pattern="[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,}$" required />   <!-- pattern : caractères suivis par un @ signe, suivi par d'autres caractères, suivi par un ".", et après le "." 2 caractères (a-z) au minimum -->
                            </div>
                            <div id="used1"></div>
                            <div class="identifiants">
                                <label class="">Nom <span class="ponctuation">d'utilisateur :</span></label>
                                <input id="user" type="text" name="pseudo" maxlength="30" pattern=".{6,}" required />     <!-- pattern : au minimum 6 caractères -->
                            </div>
                            <div id="regex1"></div><div id="used2"></div>
                            <div class="identifiants">
                                <label class="">Mot de <span class="ponctuation">passe :</span></label>
                                <input id="mdp" type="text" name="password" maxlength="40" pattern="(?=.*\d).{6,}" required />      <!-- pattern : au minimum 6 caractères dont au moins 1 chiffre -->
                            </div>
                            <div id="regex2"></div>
                        </div>
                        <div class="se-connecter">
                            <h4 class="h4-creer">Données privées :</h4>
                            <div class="identifiants">
                                <label class=""><span class="ponctuation">Prénom :</span></label>
                                <input type="text" name="prenom" maxlength="40" required />
                            </div>
                            <div class="identifiants">
                                <label class=""><span class="ponctuation">Nom :</span></label>
                                <input type="text" name="nom" maxlength="40" required />
                            </div>
                            <div class="sign signAge">
                                <div class="identifiants age">
                                    <label class="">Tu as 18 ans ou <span class="ponctuation">plus ?</span></label>
                                    <div class="inputAlias">
                                        <label class="confirm oui">
                                            Oui
                                            <input id="majeur" class="radiocheck" type="checkbox" name="majeur" value="1" checked>
                                            <span class="checkmark checkoui"></span>
                                        </label>
                                        <label class="confirm non">
                                            Non
                                            <input id="mineur" class="radiocheck" type="checkbox" name="mineur" value="1">
                                            <span class="checkmark checknon"></span>
                                        </label>
                                    </div>
                                </div>
                            </div>
                            <!--Si l'user check qu'il n'a pas 18ans, creer-compte2.js ajoute ici 2 inputs-->
                            <div id="regex3"></div>
                            <div class="sign coche2">
                                <label class="confirm">En cochant cette case, j'affirme avoir lu les Conditions d'utilisation de monDUO.ch et je les accepte.
                                    <input type="checkbox" name="cgu" value="1" required>
                                    <span class="checkmark"></span>
                                </label>
                            </div>
                            <button type="submit" id="bouton2" name="signup-btn">Créer mon compte</button>
                            <input type="hidden" id="token" name="token">
                        </form>
                        </div>
                    </fieldset>
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
    <div id="scrollMessage"><img src="images/SVG/arrow.svg"></div>
</div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/scroll.js"></script>
<script src="js/new-window.js"></script>
<script src="js/age-verification.js"></script>
<script src="js/checknewuserform.js"></script>

<script>
  grecaptcha.ready(function() {
      grecaptcha.execute('6Ld_wsEZAAAAAP3TAMsVjukyERk_QMuyL2e6Kkpp', {action: 'homepage'}).then(function(token) {
         // console.log(token);
         document.getElementById("token").value = token;
      });
  });
</script>


<?php
$email = $pseudo = $password = "";

if (isset($_POST['signup-btn'])) {
    
    $action = 'homepage';

    $curlData = array(
        'secret'	=> '6Ld_wsEZAAAAABlDwRUqMSpnHw9nH5Iln0PSXTAl',
        'response'	=> $_POST['token']
    );

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, "https://www.google.com/recaptcha/api/siteverify");
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($curlData));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $curlResponse = curl_exec($ch);

    $captchaResponse = json_decode($curlResponse, true);

    if($captchaResponse['success'] == '1'
        && $captchaResponse['score'] >= 0.5 
        && $captchaResponse['action'] == $action 
        && $captchaResponse['hostname'] ==  $_SERVER['SERVER_NAME'])
    {
        if(!isset($_POST['cgu'])) {
            die ('<div id="login"><span id="close"></span>Il faut accepter les Conditions d\'utilisation pour créer un compte DUO.</div><script src="js/messageSignin.js"></script></body></html>');
        }
        elseif
            (!isset($_POST['email']) ||
            !isset($_POST['pseudo']) ||
            !isset($_POST['password']) ||
            !isset($_POST['prenom']) ||
            !isset($_POST['nom']))
            {
                die ('<div id="login"><span id="close"></span>Il faut remplir tous les champs pour continuer et créer un compte DUO.</div><script src="js/messageSignin.js"></script></body></html>');       
        }
        else {
    
            date_default_timezone_set('Europe/Zurich');
            $error1 = $error2 = $majeur = $mineur = $nom2 = $prenom2 = 0;
    
            $email = sanitizeString($_POST['email']);
            $pseudo = sanitizeString($_POST['pseudo']);
            $password = sanitizeString($_POST['password']);
            $nom = sanitizeString($_POST['nom']);
            $prenom = sanitizeString($_POST['prenom']);
            $majeur = sanitizeString($_POST['majeur']);
            $mineur = sanitizeString($_POST['mineur']);
            $nom2 = sanitizeString($_POST['nom2']);
            $prenom2 = sanitizeString($_POST['prenom2']);
    
            $dateinscription = date("Y-m-d H:i:s");
            $hash = password_hash($password, PASSWORD_DEFAULT);
            $verificationCode = md5( rand(0,1000) );
    
    
            
            $result1 = queryMysql("SELECT * FROM utilisateurs WHERE pseudo='$pseudo'");
            $result2 = queryMysql("SELECT * FROM utilisateurs WHERE email='$email'");
    
            if ($result1->num_rows) {
                $error1 = "Le nom d'utilisateur $pseudo est déjà pris.<br>";
                die ("<div id=\"login\"><span id=\"close\"></span>$error1</div><script src=\"js/messageSignin.js\"></script></body></html>");
            }
            elseif ($result2->num_rows) {
                $error2 = 'L\'adresse email que tu as choisie est déjà utilisée pour un autre compte.<br>';
                die ("<div id=\"login\"><span id=\"close\"></span>$error2</div><script src=\"js/messageSignin.js\"></script></body></html>");
            }
            else
            {
                queryMysql("INSERT INTO utilisateurs VALUES (NULL, '$dateinscription', '$pseudo', '$email', '$hash', 0, '$verificationCode', '$dateinscription')");
                queryMysql("INSERT INTO utilisateursInformations VALUES ('$email', '$nom', '$prenom', '$majeur', '$mineur', '$nom2', '$prenom2')");
    
    
                // Mailer pour validation des comptes avec liens http://www.monduo.ch/verify.php?email=$email&verificationCode=.$verificationCode 
                $to      = $email;
                $subject = 'Confirmation d\'inscription';
                $message = 'Tu es bien inscrit sur monDUO.ch !' . "\r\n\r\n" . 
                'Ton compte sera activé et tu pourras utiliser toutes les fonctionnalités de la plateforme monDUO.ch' . "\r\n" .
                'après avoir ouvert le lien suivant :' . "\r\n" . 
                'http://www.monduo.ch/verify.php?email='.$email.'&verificationCode='.$verificationCode . "\r\n\r\n" .
                'Tes identifiants :' . "\r\n" .
                'Nom d\'utilisateur : '.html_entity_decode($pseudo). "\r\n" .
                'Mot de passe : '.html_entity_decode($password). "\r\n\r\n" .
                '© monDUO.ch - 2020' . "\r\n" .
                '(e-mail généré automatiquement, merci de ne pas y répondre)';
                $headers = 'From: noreply@monduo.ch' . "\r\n" .
                'X-Mailer: PHP/' . phpversion();
                mail($to, $subject, $message, $headers);
    
                // Info nouvelle inscription pour l'administrateur
                $toAdmin      = 'info@monduo.ch';
                $subjectAdmin = 'Nouvelle inscription sur monDUO.ch';
                $messageAdmin = 'Un nouvel utilisateur nommé ' .html_entity_decode($pseudo). ' vient de s\'inscrire avec l\'adresse e-mail '.$email. '.'."\r\n\r\n".
                '© monDUO.ch - 2020' . "\r\n" .
                '(e-mail automatique)';
                $headers = 'From: noreply@monduo.ch' . "\r\n" .
                'X-Mailer: PHP/' . phpversion();
                mail($toAdmin, $subjectAdmin, $messageAdmin, $headers);
    
    
                die ('<div id="login"><span id="close"></span><div class="title">Créer un compte - Confirmation</div><p class="msg-confirm">Un <span>mail de confirmation</span> vient d\'être envoyé à l\'adresse e-mail que tu as utilisé pour créer ton compte. <span>Clique sur le lien</span> mentionné dans ce mail pour confirmer et finaliser la création de ton compte.</p></div></div><script src="js/messageSignin.js"></script></body></html>');
            }
        }
    }
    else{
        die ('<div id="login"><span id="close"></span>Cette inscription a été détecté comme étant l\'oeuvre d\'un robot.<br>Si c\'est une erreur, tu peux essayer de remplir à nouveau le formulaire d\'inscription.<br>Si le problème persiste tu peux nous envoyer un message via la page <a id="lienvers" href="contact.php">Nous contacter</a>.</div><script src="js/messageSignin.js"></script></body></html>');       
    }
    
}
?>
</body>
</html>