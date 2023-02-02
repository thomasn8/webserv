<?php
  $dbhost  = 'cw72y.myd.infomaniak.com';    // Unlikely to require changing
  $dbname  = 'cw72y_monDUO2020';
  $dbuser  = 'cw72y_thomasN20';
  $dbpass  = 'duoDENUM2020';

  $connection = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
  if ($connection->connect_error) die("Erreur 1 : Désolé, un problème est survenu. Réessaie 
  et si ça persiste, contacte-nous directement à info@monduo.ch");

function destroySession()
  {
    $_SESSION=array();

    if (session_id() != "" || isset($_COOKIE[session_name()]))
      setcookie(session_name(), '', time()-2592000, '/');

    session_destroy();
  }

if (isset($_SESSION['LAST_ACTIVITY']) && (time() - $_SESSION['LAST_ACTIVITY'] > 129600)) {
  // if the last request was more than 60*60*36 minutes ago (= 36h)
  session_unset();     // unset $_SESSION variable for the run-time 
  destroySession();   // destroy session data in storage
}

function sanitizeString($var)           //nettoie les inputs utilisateurs
  {
  if (get_magic_quotes_gpc())
  $var = stripslashes($var);
	$var = htmlentities($var);
  $var = strip_tags($var);
  $var = str_replace("'", "~", $var);   //MySQL n'accepte pas l'injection d'apostrope " ' " alors remplace les ' par ~ 
	return $var;
  }

function apostrophe($phrase)          //transforme les ~ (de la base de données) en ' [A UTILISER SUR LES MESSAGES ENTRE USERS]
  {
    $phrase = str_replace("~", "'", $phrase);
    return $phrase;
  }

?>