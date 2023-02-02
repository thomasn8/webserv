<?php
session_start();
require_once 'fonctions.php';

if (isset($_SESSION['pseudo'])) {

    $userPseudo = $_SESSION['pseudo'];
    
    $result = queryMysql("SELECT email FROM utilisateurs WHERE pseudo = '$userPseudo'");
    $rows = $result->num_rows;
    if ($rows > 1) die('erreur');
    else {
        $row = $result->fetch_array(MYSQLI_ASSOC);
        
        $userEmail = $row['email'];
    }
}



if (isset($_POST['change-btn1'])) {

    $nouvelEmail = sanitizeString($_POST['nouvel-email']);
    $password = sanitizeString($_POST['password']);


    $result = queryMySQL("SELECT * FROM utilisateurs WHERE pseudo = '$userPseudo'");
    $user = $result->fetch_array(MYSQLI_ASSOC);
    $result->close();

    //si le mot de passe ne match pas avec l'identifiant
    if ((password_verify($password, $user['password'])) == false) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Mot de passe incorrect.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    //si tout est ok
    else {
        queryMySQL("UPDATE utilisateurs SET email='$nouvelEmail' WHERE pseudo = '$userPseudo'");
        queryMySQL("UPDATE utilisateursInformations SET email='$nouvelEmail' WHERE email = '$userEmail'");

        echo ("<div id=\"login\"><span id=\"close\"></span>Identifiant mis à jour.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
}



if (isset($_POST['change-btn2'])) {
    
    $password = sanitizeString($_POST['ancien-password']);
    $nouveauPassword = sanitizeString($_POST['nouveau-password']);
    $nouveauPassword2 = sanitizeString($_POST['nouveau-password-bis']);


    $result = queryMySQL("SELECT * FROM utilisateurs WHERE pseudo = '$userPseudo'");
    $user = $result->fetch_array(MYSQLI_ASSOC);
    $result->close();
    
    //si le mot de passe ne match pas avec l'identifiant
    if ((password_verify($password, $user['password'])) == false) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Ancien mot de passe incorrect.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    //si la confirmation du nouveau mot de passe ne match pas
    elseif ($nouveauPassword !== $nouveauPassword2) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Erreur : nouveau mot de passe mal répété.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    elseif (strlen($nouveauPassword) < 6) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Ton mot de passe doit contenir au minium 6 caractères dont 1 chiffre.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    elseif (!preg_match("/[a-z]/", $nouveauPassword)) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Ton mot de passe doit contenir au minium 6 caractères dont 1 chiffre.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    elseif (!preg_match("/[0-9]/", $nouveauPassword)) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Ton mot de passe doit contenir au minium 6 caractères dont 1 chiffre.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
    //si tout est ok
    else {
        $hashPassword = password_hash($nouveauPassword, PASSWORD_DEFAULT);

        queryMySQL("UPDATE utilisateurs SET password='$hashPassword' WHERE pseudo = '$userPseudo'");

        echo ("<div id=\"login\"><span id=\"close\"></span>Identifiant mis à jour.</div><script src=\"js/messageClose.js\"></script></body></html>");
    }
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>
   
    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Modifier et récupérer ses identifiants - monDUO.ch</title>
    <meta name="description" content="Récupérer un mot passe, changer d'identifiant ou modifier ses informations privées."/>
    <link rel="canonical" href="https://www.monduo.ch/recup-mdp.php/" />
    <meta name="robots" content="noindex,follow" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Modifier et récupérer ses identifiants - monDUO.ch" />
	<meta property="og:description" content="Récupérer un mot passe, changer d'identifiant ou modifier ses informations privées." />
	<meta property="og:url" content="https://www.monduo.ch/recup-mdp.php/" />
	<meta property="og:site_name" content="monDUO.ch" />
	<meta property="article:modified_time" content="2020-09-09T21:48:59+00:00" />
	<meta property="og:image" content="https://www.monduo.ch/images/icone.png" />

   <link rel="icon" type="image/png" href="images/icone.png">
   <link rel="stylesheet" href="style.css">
   <link rel="stylesheet" href="recup-mdp.css">
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

$result = queryMysql("SELECT email FROM utilisateurs WHERE pseudo = '$userPseudo'");
$rows = $result->num_rows;
if ($rows > 1) die('erreur');
else {
    $row = $result->fetch_array(MYSQLI_ASSOC);
    
    $userEmail = $row['email'];
}

echo <<<_END
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter">
                        <div class="title">Mes infos privées</div>
                        <h5>Modifier les identifiants liés à mon compte.</h5>
                        <fieldset id="publier" class="fieldset-connexion">
                            <legend id="ton-annonce">$userPseudo</legend>
                            <div class="changer">
                                <div class="id-priv">
                                    <div class="id1"><span class="ponctuation">E-mail :</span></div>
                                    <div id="email" class="id2">$userEmail</div>
                                    <a id="change1" class="changeId" href="change-mail">Changer l'adresse e-mail</a>
                                </div>
                                <div class="id-priv">
                                    <div class="id1">Mot <span class="ponctuation">de passe :</span></div>
                                    <div class="id2">par mesure de sécurité le mot de passe n'est pas affiché</div>
                                    <a id="change2" class="changeId" href="change-password">Changer mon mot de passe</a>
                                </div>
                                <div class="id-priv">
                                    <a id="$userPseudo" class="deleteAccount" href="deleteAccount">Effacer mon compte et toutes les données liées</a>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
            </section>
_END;
}
//SI PAS DE SESSION USER ACTIVE
else {
echo <<<_END
    <section class="section-sec" id="log">
        <div class="connection">
            <div class="se-connecter">
                <div class="title">Récupérer mon mot de passe</div>
                <h5>Demander un nouveau mot de passe à l'adresse e-mail de mon compte.</h5>
                <fieldset id="publier" class="fieldset-connexion">
                    <legend id="ton-annonce">Identifiant</legend>
                    <form id="" action="recup-mdp.php" method="post">
                        <div class="identifiants">
                            <label class="">Nom d'utilisateur ou <span class="ponctuation">adresse e-mail :</span></label>
                            <input id="identifiant" type="text" name="identifiant" maxlength="80" required/>
                        </div>
                        <h5 id="errorLog"><span id="errorMsg"></span></h5>
                        <button id="mailMDP" type="submit" name="mailMDP">demander</button>
                    </form>
                </fieldset>
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
</div>
<div id="scrollMessage"><img src="images/SVG/arrow.svg"></div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/scroll.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/new-window.js"></script>
<?php
if (isset($_SESSION['pseudo'])) {
    echo '<script src="js/changeIdentifiants.js"></script>';
    echo '<script src="js/effacerCompte.js"></script>';
}
else {
    //echo '<script src="js/xxx.js"></script>';
}
?> 
</body>
</html>

<?php
if ((!isset($_SESSION['pseudo'])) && (isset($_POST['mailMDP'])) && isset($_POST['identifiant'])) {

    $identifiant = sanitizeString($_POST['identifiant']);

    $result = queryMySQL("SELECT email FROM utilisateurs WHERE pseudo = '$identifiant' OR email ='$identifiant'");

    if(strlen($identifiant) < 6){
        
        die ('<div id="login"><span id="close"></span><div class="title">Identifiant non valide</div><p class="msg-confirm">L\'identifiant que tu as entré n\'est pas valide. Réessaie et si le problème persiste tu peux <a class="lien-contact" href="contact.php">nous contacter</a>.</p></div></div><script src="js/messageRecup-mdp.js"></script></body></html>');
    }
    elseif ($result->num_rows == 0) {
        
        die ('<div id="login"><span id="close"></span><div class="title">Identifiant inconnu</div><p class="msg-confirm">Aucun compte n\'existe avec cet identifiant. Réessaie ou, si tu ne connais plus tes identifiants, tu peux <a class="lien-contact" href="contact.php">nous contacter</a>.</p></div></div><script src="js/messageRecup-mdp.js"></script></body></html>');
    }
    else {
        $user = $result->fetch_array(MYSQLI_ASSOC);
        $result->close();

        $email = $user['email'];
        
        function randomPassword() {
            $alphabet = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
            $pass = array(); //remember to declare $pass as an array
            $alphaLength = strlen($alphabet) - 1; //put the length -1 in cache
            for ($i = 0; $i < 15; $i++) {
                $n = rand(0, $alphaLength);
                $pass[] = $alphabet[$n];
            }
            return implode($pass); //turn the array into a string
        }
        $randomPassword = randomPassword();
        $hash = password_hash($randomPassword, PASSWORD_DEFAULT);


        queryMySQL("UPDATE utilisateurs SET password = '$hash' WHERE email = '$email'");

        
        // Mailer pour envoyer le nouveau password 
        $to      = $email;
        $subject = 'Nouveau mot de passe généré automatiquement';
        $message = 'Voici le nouveau mot de passe attribué à ton compte. Tu peux l\'utiliser pour te reconnecter sur ton compte : ' . "\r\n" .$randomPassword . "\r\n\r\n" . 'Afin d\'avoir un mot de passe mémorisable, pense à le modifier le plus vite possible sur la page www.monduo.ch/mon-compte.php, onglet "modifier mes informations privées".' . "\r\n\r\n" . '© monDUO.ch - 2020'  . "\r\n" . '(e-mail généré automatiquement, merci de ne pas y répondre)';
        $headers = 'From: noreply@monduo.ch' . "\r\n" .
        'X-Mailer: PHP/' . phpversion();
        mail($to, $subject, $message, $headers);


        die ('<div id="login"><span id="close"></span><div class="title">Nouveau mot de passe attribué à ton compte</div><p class="msg-confirm">Un nouveau mot de passe a été généré automatiquement et envoyé à l\'adresse e-mail de ton compte. Tu peux l\'utiliser afin de te reconnecter et ensuite le changer sous la rubrique <a class="lien-contact" href="mon-compte.php">Mon profil - modifier mes informations privées</a>.</p></div></div><script src="js/messageRecup-mdp2.js"></script></body></html>');
    }    
}
?>