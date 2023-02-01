$(function() {
    $(".supprimer-annonce-oui").on('click', function() {

        var pseudo = $('div.pseudo').attr('id');
        var deleteAccount = { "deleteAccount": pseudo };

        $.ajax({
            type: "POST", 
            url: "effacerCompte.php",
            data: deleteAccount,
            success: function() {
                window.location.replace("https://monDUO.ch/mon-compte.php");      
            },
            fail: function(){
                $('div#ouiOuNon').html('Erreur. Réessaie et si ça ne fonctionne toujours pas contacte-nous directement à info@monduo.ch');
            }
        });
    })

    $(".supprimer-annonce-non").on('click', function() {
        $('#login').remove();
    })
})