$(function() {
    $("#change1").on('click', function() {

        event.preventDefault();
        
        var alerte =  '<div id="login">';
        alerte +=         '<span id="close"></span>';
        alerte +=         '<div class="title annonceNO">Changer l\'adresse e-mail de mon compte</div>';
        alerte +=         '<h5>Identifiants : <span id="errorMsg"></span></h5>';
        alerte +=         '<form id="publish2" action="recup-mdp.php" method="post">';
        alerte +=             '<div class="identifiants">';
        alerte +=                 '<label>Nouvelle adresse <span class="ponctuation">e-mail :</span></label>';
        alerte +=                 '<input id="identifiant1" type="email" name="nouvel-email" maxlength="80" required />';
        alerte +=             '</div>';
        alerte +=             '<div class="identifiants">';
        alerte +=                 '<label class="">Mot de <span class="ponctuation">passe :</span></label>';
        alerte +=                 '<input id="identifiant2" type="password" name="password" maxlength="30" required />';
        alerte +=             '</div>';
        alerte +=             '<button id="change-btn1" type="submit" name="change-btn1">Changer</button>';
        alerte +=         '</form>';
        alerte +=     '</div>';

        $('#login').remove();
        $("body").append(alerte);
        $("body").append('<script src="js/messageClose.js"></script>');
    })

    $("#change2").on('click', function() {
        
        event.preventDefault();

        var alerte =  '<div id="login">';
        alerte +=         '<span id="close"></span>';
        alerte +=         '<div class="title annonceNO">Changer de mot de passe</div>';
        alerte +=         '<h5>Identifiants : <span id="errorMsg"></span></h5>';
        alerte +=         '<form id="publish2" action="recup-mdp.php" method="post">';
        alerte +=             '<div class="identifiants">';
        alerte +=                 '<label>Ancien mot de <span class="ponctuation">passe :</span></label>';
        alerte +=                 '<input id="identifiant3" type="password" name="ancien-password" maxlength="30" required />';
        alerte +=             '</div>';
        alerte +=             '<div class="identifiants">';
        alerte +=                 '<label class="">Nouveau mot de <span class="ponctuation">passe :</span></label>';
        alerte +=                 '<input id="identifiant4" type="password" name="nouveau-password" maxlength="30" required />';
        alerte +=             '</div>';
        alerte +=             '<div class="identifiants">';
        alerte +=                 '<label class="">Confirmation du nouveau mot de <span class="ponctuation">passe :</span></label>';
        alerte +=                 '<input id="identifiant5" type="password" name="nouveau-password-bis" maxlength="30" required />';
        alerte +=             '</div>';
        alerte +=             '<button id="change-btn2" type="submit" name="change-btn2">Changer</button>';
        alerte +=         '</form>';
        alerte +=     '</div>';

        $('#login').remove();
        $("body").append(alerte);
        $("body").append('<script src="js/messageClose.js"></script>');
    })
})