<?php // Example 27-6: checkuser.php
  require_once 'fonctions.php';

  if (isset($_POST['email']))
  {
    $email   = sanitizeString($_POST['email']);
    $result = queryMysql("SELECT * FROM utilisateurs WHERE email='$email'");

    if ($result->num_rows)
      echo  "<span class='taken'>&nbsp;&#x2718; " .
            "L'adresse email '$email' est déjà prise</span>";
    else
      echo "";
  }
?>
