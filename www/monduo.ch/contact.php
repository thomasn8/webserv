<?php
require_once 'fonctions.php';
function sanitizeString2($var)
  {
  if (get_magic_quotes_gpc())
  $var = stripslashes($var);
  $var = strip_tags($var);
	return $var;
  }

echo <<<_END
<!DOCTYPE html>
<html lang="fr">
<head>

    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Nous contacter - monDUO.ch</title>
    <meta name="description" content="Sur cette page tu peux contacter directement. Nous y répondrons le plus rapidement possible."/>
    <link rel="canonical" href="https://www.monduo.ch/contact.php/" />

    <meta property="og:locale" content="fr_FR" />
    <meta property="og:type" content="website" />
    <meta property="og:title" content="Nous contacter - monDUO.ch" />
    <meta property="og:description" content="Sur cette page tu peux contacter directement. Nous y répondrons le plus rapidement possible." />
    <meta property="og:url" content="https://www.monduo.ch/contact.php/" />
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
           <a href="index.html">Page d'accueil</a>
           <a href="publier.php">Publier une annonce</a>
           <a href="voir.php">Voir les annonces</a>
           <a href="mon-compte.php">Mon compte</a>
        </nav>
    </header>
    <div class="container">
        <main class="main-sec">
            <section class="section-sec">
                <div class="title" id="contacter">Nous contacter</div>
                <div class="formulaire">		
                    <form method="post" action="contact.php">
                        <fieldset id="contact">
                            <legend id="ton-annonce">@ monduo.ch</legend>
                            <div class="champs">
                                <label class="lab">Prénom :</label>
                                <input type="text" name="prenom" maxlength="40" required />
                            </div>
                            <div class="champs">
                                <label class="lab">Nom :</label>
                                <input type="text" name="nom" maxlength="40" required />
                            </div>
                            <div class="champs">
                                <label class="lab">Téléphone :</label>
                                <input type="tel" name="tel" 						
                                pattern="(0041|041|\+41|\+\+41|41)?(0|\(0\))?([1-9]\d{1})(\d{3})(\d{2})(\d{2})" required />
                            </div>
                            <div class="champs">
                                <label class="lab">E-mail :</label>
                                <input type="email" name="email" maxlength="40" pattern="[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,}$" required />
                            </div>
                            <div class="champs facultatif">
                                <label class="lab adr">Adresse :</label>
                                <input type="text" name="adresse1" maxlength="80" />
                            </div>
                            <div class="champs facultatif">
                                <label class="lab">Localité :</label>
                                <input type="text" name="adresse3" maxlength="60" />
                            </div>
                            <div class="champs facultatif">
                                <label class="lab" id="postal">Code postal :</label>
                                <input type="text" name="adresse4" maxlength="6" />
                            </div>
                            <div class="area">	
                                <label class="lab">Ton message :</label>
                                <textarea type="text" name="message" maxlength="4000" required></textarea>
                            </div>
                        <div>
                            <button type="submit" id="boutonforumalaire" disabled>Envoyer</button>
                        </fieldset>
                        </div>
                    </form>
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
<script src="js/new-window.js"></script>
<script src="js/nav-activ.js"></script>
</body>
</html>
_END;

if(isset($_POST['prenom'])) 
{

    if(!isset($_POST['prenom']) ||
        !isset($_POST['nom']) ||
        !isset($_POST['tel']) ||
        !isset($_POST['email']) ||
		!isset($_POST['message'])) {
        echo ("<div id=\"login\"><span id=\"close\"></span>Désolé mais il semblerait qu'il y ait eu un problème avec l'envoi du formulaire. Tu peux réessayer 
        et, si ça ne fonctionne toujours pas, contacte-nous à info@monduo.ch</div><script src=\"js/message.js\"></script></body></html>");
    }
    die ("<div id=\"login\"><span id=\"close\"></span>Désolé mais il semblerait qu'il y ait eu un problème avec l'envoi du formulaire. Tu peux réessayer 
    et, si ça ne fonctionne toujours pas, contacte-nous à info@monduo.ch</div><script src=\"js/message.js\"></script></body></html>");

    /*
    date_default_timezone_set('Europe/Zurich');
    $dateContact = date("Y-m-d H:i:s");

    $prenom = sanitizeString($_POST['prenom']);
    $nom = sanitizeString($_POST['nom']);
    $tel = sanitizeString($_POST['tel']);
    $email = sanitizeString($_POST['email']);
    $adresse1 = sanitizeString($_POST['adresse1']);
    $adresse3 = sanitizeString($_POST['adresse3']);
    $adresse4 = sanitizeString($_POST['adresse4']);
    
    $messageEmail = sanitizeString2($_POST['message']);
    $messageDB = sanitizeString($messageEmail);
 
    

    $email_message = "Formulaire de contact - " . $dateContact . "\n\n"; 
 
    $email_message .= "Prénom:  " . $prenom . "\n";
    $email_message .= "Nom : " . $nom . "\n";
    $email_message .= "Adresse : " . $adresse1 . "\n";
    $email_message .= "Localité : " . $adresse4 . " " . $adresse3 . "\n";
    $email_message .= "Numéro de téléphone : " . $tel . "\n";
    $email_message .= "E-mail : " . $email . "\n\n";
    $email_message .= "Message :\n" . $messageEmail . "\n\n";
    $email_message .= "© monDUO.ch - 2020";
 


    $email_to = "info@monduo.ch";
    $email_subject = "Contact monDUO";
    // create email headers
    $headers = 'From: '.$email."\r\n".
    'Reply-To: '.$email."\r\n" .
    'X-Mailer: PHP/' . phpversion();
    @mail($email_to, $email_subject, $email_message, $headers);


    //queryMysql("INSERT INTO contacts VALUES(NULL, '$dateContact', '$prenom', '$nom', '$tel', '$email', '$adresse1', '$adresse3', '$adresse4', '$messageDB')");
    

    die ('<div id="login"><span id="close"></span> Nous avons bien reçu ton message.<br><br>Nous ferons le maximum pour répondre à ta demande au plus vite.</div><script src="js/message.js"></script></body></html>');
    */
}

echo <<<_END

    </body>
</html>
_END;
?>