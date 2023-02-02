<?php
session_start();
require_once 'fonctions2.php';

function queryMysql($query)
  {
    global $connection;
    $result = $connection->query($query);
    if (!$result) die ('<div id="login"><span id="close"></span>Nous n\'acceptons qu\'une suggestion par personne. Merci quand même !</div><script src="js/messageSignin.js"></script></body></html>');
    return $result;
  }
?>

<!DOCTYPE html>
<html lang="fr">
<head>
   
    <meta name="language" content="fr" /> 
    <meta charset="UTF-8">
    <meta name="viewport" content="width=  , initial-scale=1.0">

    <title>Suggérer une nouvelle discipline - monDUO.ch</title>
    <meta name="description" content="La discipline que tu cherches n'existe pas ? Sur cette page tu peux suggérer une nouvelle discipline."/>
    <link rel="canonical" href="https://www.monduo.ch/suggestions.php/" />
    <meta name="robots" content="noindex,follow" />

    <meta property="og:locale" content="fr_FR" />
	<meta property="og:type" content="website" />
	<meta property="og:title" content="Suggérer une nouvelle discipline - monDUO.ch" />
	<meta property="og:description" content="La discipline que tu cherches n'existe pas ? Sur cette page tu peux suggérer une nouvelle discipline." />
	<meta property="og:url" content="https://www.monduo.ch/suggestions.php/" />
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
            <section class="section-sec">
                <div class="propulser">
                    <div class="title">Propulser monDUO sur le net</div>
                    <div class="propuls-text">
                            A l'heure actuelle, la plateforme monDUO.ch est en phase de développement. Pour aider monDUO.ch à décoller et à fonctionner 
                            de façon autonome et gratuite, tu peux par exemple parler de cette nouvelle plateforme autour de toi et/ou partager notre page 
                            <a class="partage" href="https://www.facebook.com/Monduoch-653782461899658/">facebook</a> / <a class="partage" href="https://www.linkedin.com/company/monduo-ch/">linkedIn</a> sur les réseaux sociaux.
                        </div>
                    <div class="propuls-img"> 
                        <img src="images/SVG/presentation/rocket.svg">
                        <img src="images/SVG/presentation/share.svg">
                </div>
            </section>

            
            <section class="section-sec" id="log">
                <div class="connection">
                    <div class="se-connecter"><form id="suggest" action="suggestions.php" method="post">
                        <div class="title">Suggestions</div>
                        <h5>Ici, tu peux suggérer une nouvelle discipline :</h5>
                        <div id="correction2-padding" class="identifiants identifiants-select retrait3">
                            <label class=""><span class="ponctuation">Domaine :</span></label>
                            <select id="" name="domaineID" required>
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
                        <div class="identifiants">
                            <label class=""><span class="ponctuation">Discipline :</span></label>
                            <input id="" type="text" name="suggestion" maxlength="40" required />
                        </div>
                        <div class="identifiants">
                            <label class="">Commentaire ou <span class="ponctuation">description :</span></label>
                            <input id="" type="text" name="commentaire" maxlength="80" />
                        </div>
                        <?php
                            if (!isset($_SESSION['pseudo'])) {   
                                echo <<<_END
                                <div class="identifiants">
                                    <label class=""><span class="ponctuation">Prénom :</span></label>
                                    <input id="" type="text" name="prenom" maxlength="40" required />
                                </div>
                                <div class="identifiants">
                                    <label class=""><span class="ponctuation">Email :</span></label>
                                    <input id="" type="email" name="email" maxlength="80" required />
                                </div>
_END;
                            }
                        ?>
                        <button type="submit" name="suggest-btn">envoyer</button>
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
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="js/nav-activ.js"></script>
<script src="js/scroll.js"></script>
<script src="js/new-window.js"></script>
<?php
if (isset($_POST['suggest-btn'])) {

    if
        (!isset($_POST['domaineID']) &&
        !isset($_POST['matiere']) &&
        (!isset($_POST['email']) || !isset($_SESSION['email'])) &&
        (!isset($_POST['prenom']) || !isset($_SESSION['pseudo']))) 
        {
            die ('<div id="login"><span id="close"></span>Il faut remplir tous les champs requis pour faire une suggestion.</div><script src="js/messageSignin.js"></script></body></html>');       
    }
    else {

        date_default_timezone_set('Europe/Zurich');
        $date = date("Y-m-d H:i:s");

        $domaineID = sanitizeString($_POST['domaineID']);
        $result = queryMysql("SELECT domaine FROM domaines WHERE domaineID = '$domaineID'");
        $row = $result->fetch_array(MYSQLI_ASSOC);
        $domaine = $row['domaine'];

        $suggestion = sanitizeString($_POST['suggestion']);
        $commentaire = sanitizeString($_POST['commentaire']);

        if (isset($_SESSION['pseudo'])) {
            $pseudo = $_SESSION['pseudo'];
            $email = $_SESSION['email'];
        }
        else {
            $pseudo = sanitizeString($_POST['prenom']);
            $email = sanitizeString($_POST['email']);
        }

        queryMysql("INSERT INTO suggestions VALUES (NULL, '$date', '$domaine', '$domaineID', '$suggestion', '$commentaire', '$pseudo', '$email')");
        echo ('<div id="login"><span id="close"></span>Merci pour ta suggestion !</div><script src="js/messageSignin.js"></script></body></html>');
    }
}
?>

</body>
</html>