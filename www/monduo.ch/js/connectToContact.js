$(function() {
    $(".contactButton").on('click', function() {

        var connexion =  '<div id="login">';
        connexion +=         '<span id="close"></span>';
        connexion +=         '<div class="title">Connexion</div>';
        connexion +=         '<h5>Identifiants : <span id="errorMsg"></span></h5>';
        connexion +=         '<form id="connexion" action="loginAndCreateSession" method="post">';
        connexion +=             '<div class="identifiants">';
        connexion +=                 '<label>Nom d\'utilisateur ou adresse <span class="ponctuation">e-mail :</span></label>';
        connexion +=                 '<input id="identifiant1" type="text" name="identifiant" maxlength="80" autocomplete="on" required />';
        connexion +=             '</div>';
        connexion +=             '<div class="identifiants">';
        connexion +=                 '<label class="">Mot de <span class="ponctuation">passe :</span></label>';
        connexion +=                 '<input id="identifiant2" type="password" name="password" maxlength="30" autocomplete="on" required />';
        connexion +=             '</div>';
        connexion +=              '<h5 class="oublies"><a class="recup-mdp" href="recup-mdp.php">Identifiants oubliés ?</a></h5>';
        connexion +=             '<button id="log-btn" type="submit" name="log-btn">Connexion</button>';
        connexion +=         '</form>';
        connexion +=     '</div>';

        $('#login').remove();
        $("body").append(connexion);
        $("body").append('<script src="js/messageClose.js"></script>');
        $("body").append('<script src="js/loginAndCreateSession.js"></script>');
    });
})