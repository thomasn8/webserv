$(function() {
    $(".mon-annonce-btn1").on('click', function() {

        var annonceNO = $(this).attr('id');
        
        var alerte =  '<div id="login">';
        alerte +=         '<span id="close"></span>';
        alerte +=         '<div id="' + annonceNO + '" class="title annonceNO">Supprimer cette annonce ?</div>';
        alerte +=         '<div id="ouiOuNon">';
        alerte +=             '<button class="supprimer-annonce-oui" name="log-btn">Oui</button>';
        alerte +=             '<button class="supprimer-annonce-non" name="log-btn">Non</button>';
        alerte +=         '</div>';
        alerte +=     '</div>';

        $("body").append(alerte);
        $("body").append('<script src="js/messageClose.js"></script>');
        $("body").append('<script src="js/supprimerOui-non.js"></script>');
    })

    $(".mon-annonce-btn2").on('click', function() {
        
        var annonceNO = $(this).attr('id');

        var annonceUpdate = { "annonceUpdate": annonceNO };

        $.ajax({
            type: "POST", 
            url: "supprimer-prolonger-monAnnonce.php",
            data: annonceUpdate,
            success: function() {
                location.reload();
            }
        });
    })
})