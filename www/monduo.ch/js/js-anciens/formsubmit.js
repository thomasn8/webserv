$("#publish").submit(function( event ) {

    event.preventDefault();


    var loginDetails = '<div id="login">';

    loginDetails += '<div class="title">Publier une annonce</div>';

    loginDetails += '<h5>Login : <span><?php $error ?></span></h5>';

    loginDetails += '<div class="identifiants">';
    loginDetails += '<label>Nom d\'utilisateur ou adresse <span class="ponctuation">e-mail :</span></label>';
    loginDetails += '<input type="text" name="identifiant" maxlength="60" autocomplete="on" required /></div>';

    loginDetails += '<div class="identifiants">';
    loginDetails += '<label class="">Mot de <span class="ponctuation">passe :</span></label>';
    loginDetails += '<input type="text" name="password" maxlength="30" autocomplete="on" pattern="(?=.*\d).{6,}" required /></div>';

    loginDetails += '<button type="submit" name="log-btn">publier</button>';

    
    $("#publish").append(loginDetails);
});

/*
$("#publish").submit(function( event ) {

    event.preventDefault();

    //append dans le form#publish la div #login avec deux nouvelles inputs et un button name"log-btn"
    var loginDetails = $('<div id="login"><div class="title">Publier une annonce</div><h5>Login : <span><?php $error ?></span></h5><div class="identifiants"><label>Nom d\'utilisateur ou adresse <span class="ponctuation">e-mail :</span></label><input type="text" name="identifiant" maxlength="60" autocomplete="on" required /></div><div class="identifiants"><label class="">Mot de <span class="ponctuation">passe :</span></label><input type="text" name="password" maxlength="30" autocomplete="on" pattern="(?=.*\d).{6,}" required /></div><button type="submit" name="log-btn">publier</button>');
    
    $("#publish").append(loginDetails);
});
*/