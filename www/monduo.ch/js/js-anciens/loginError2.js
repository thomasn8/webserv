$(function() {
    $("#publish2").submit(function(event) {
        event.preventDefault();

        var identifiants = { "identifiant1": $("#identifiant1").val(), "identifiant2": $("#identifiant2").val(), };
        var test;

        $.ajax({
            type: "GET", 
            url: "loginError.php",
            data: identifiants,
            success: function(result) {
                if (result !== '') {
                    test = result;
                }
                else {
                    test = 0;
                }
            }
        });

        if (test !== 0){
            alert(test)
        }
        else { alert('OKAY')}

        /*
        if ($('span#errorMsg').not(':empty')) { 
            //event.preventDefault();
            $("span#errorMsg").css({'textDecoration':'underline'})
        }*/
        // IMPOSSIBLE D'ENVOYER LE FORMULAIRE EN AYANT APPELER CETTE FONCTION ...
        // TROUVER UN AUTRE MOYEN
    });
})