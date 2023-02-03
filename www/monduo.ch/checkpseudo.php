<?php // Example 27-6: checkuser.php
  require_once 'fonctions.php';

  if (isset($_POST['pseudo']))
  {
    $pseudo   = sanitizeString($_POST['pseudo']);
    $result = queryMysql("SELECT * FROM utilisateurs WHERE pseudo='$pseudo'");

    if ($result->num_rows)
      echo  "<span class='taken'>&nbsp;&#x2718; " .
            "Le pseudo '$pseudo' est déjà pris</span>";
    else
      echo "<span class='available'>&nbsp;&#x2714; " .
           "Le pseudo '$pseudo' est libre</span>";
  }
?>