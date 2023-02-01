$("#publish").submit(function(event) {

    event.preventDefault();

    $(".form1").attr('form', 'publish2');

    var publish2 =  '<div id="login">';
    publish2 +=         '<div class="title">Publier une annonce</div>';
    publish2 +=         '<span id="close"></span>';
    publish2 +=         '<h5>Identifiants : <span id="errorMsg"></span></h5>';
    publish2 +=         '<form id="publish2" action="publier.php" method="post">';
    publish2 +=             '<div class="identifiants">';
    publish2 +=                 '<label>Nom d\'utilisateur ou adresse <span class="ponctuation">e-mail :</span></label>';
    publish2 +=                 '<input id="identifiant1" type="text" name="identifiant" maxlength="80" autocomplete="on" required />';
    publish2 +=             '</div>';
    publish2 +=             '<div class="identifiants">';
    publish2 +=                 '<label class="">Mot de <span class="ponctuation">passe :</span></label>';
    publish2 +=                 '<input id="identifiant2" type="password" name="password" maxlength="30" autocomplete="on" required />';
    publish2 +=             '</div>';
    publish2 +=             '<h5 class="oublies"><a class="recup-mdp" href="recup-mdp.php">Identifiants oubliés ?</a></h5>';
    publish2 +=             '<button id="button2" type="submit" name="log-btn">Publier</button>';
    publish2 +=         '</form>';
    publish2 +=     '</div>';

    $("body").append(publish2);
    $("body").append('<script src="js/messageClose.js"></script>');
    $("body").append('<script src="js/loginAndPost.js"></script>');
});