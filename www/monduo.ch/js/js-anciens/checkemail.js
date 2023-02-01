$('#email').on('blur', function() {
    email = this
    if (((email.value.indexOf(".") > 0) && (email.value.indexOf("@") > 0)) || /[^a-zA-Z0-9.@_-]/.test(email.value)) {   //si l'adresse mail est au bon format...
        if (email.value == '')  
        {
            $('#used1').html('')
            return
        }
        else{           //...contrôle si l'adresse mail est déjà prise ou non
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
    else{           //... sinon (si elle n'est pas au bon format) affiche un message d'erreur
        $('#used1').html('Adresse email invalide').removeClass().addClass('taken').hide().fadeIn(400);
    }
})