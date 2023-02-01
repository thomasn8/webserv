
//CHECK LES INPUTS ET AFFICHE DES MESSAGES D'ERREUR OU DE VALIDATION EN FONCTION DES CARACTERES AUTORISES

//check si le pseudo est pris/libre
$('#user').on('input', function() {
    var pseudo = this

    if (pseudo.value == '') {
        $('#used2').html('');
        return
    }
    else{
        $.post
        (
            'checkpseudo.php',
            { pseudo : pseudo.value },
            function(data)
            {
            $('#used2').html(data).hide().fadeIn(400);
            }
        )
    }
})
//check si le pseudo est au bon format
$('#user').on('input', function() { 
    var conditions = '&nbsp;&#x2718; Ton pseudo doit contenir au moins 6 caractères (caractères spéciaux autorisés : .-_ )';
    var valide = '&nbsp;&#x2714; Pseudo valable'

    var $input = $('#user').val();

    if($input.length < 6) { 
        $('#regex1').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if(/[^a-zA-Z0-9éèêàáâäôûüïîñç ._-]/.test($input)) { 
        $('#regex1').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else { 
        $('#regex1').html(valide).removeClass().addClass('available').hide().fadeIn(400);
    }
})


//check si l'adresse mail est prise/libre et si elle est au bon format 
$('#email').on('blur', function() {
    var email = this
    if (((email.value.indexOf(".") > 0) && (email.value.indexOf("@") > 0)) || /[^a-zA-Z0-9.@_-]/.test(email.value)) {   //si l'adresse mail est au bon format...
        if (email.value == '')  
        {
            $('#used1').html('')
            return
        }
        else {           //...contrôle si l'adresse mail est déjà prise ou non
            $.post
            (
                'checkemail.php',
                { email : email.value },
                function(data)
                {
                $('#used1').html(data).hide().fadeIn(400);
                }
            )
        }
    }
    else {           //... sinon (si elle n'est pas au bon format) affiche un message d'erreur
        $('#used1').html('Adresse email invalide').removeClass().addClass('taken').hide().fadeIn(400);
    }
})



//check si le password est au bon format
$('#mdp').on('input', function() {
    var conditions = '&nbsp;&#x2718; Ton mot de passe doit contenir au moins 6 caractères dont 1 chiffre (caractères spéciaux autorisés : .-_ )';
    var valide = '&nbsp;&#x2714; Mot de passe valable';

    var $input = $('#mdp').val();

    if($input.length < 6) { 
        $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if(!/[0-9]/.test($input)) { 
        $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if(/[^a-zA-Z0-9éèêàáâäôûüïîñç._-]/.test($input)) { 
        $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else { 
        $('#regex2').html(valide).removeClass().addClass('available').hide().fadeIn(400);
    }
})