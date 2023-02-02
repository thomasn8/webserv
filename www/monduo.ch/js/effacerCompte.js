$(function() {
    $("a.deleteAccount").on('click', function() {
            
        event.preventDefault();

        var pseudo = $(this).attr('id');
        
        var alerte =  '<div id="login">';
        alerte +=         '<span id="close"></span>';
        alerte +=         '<div id="'+pseudo+'" class="title pseudo">Effacer mon compte</div>';
        alerte +=         '<h5>Je souhaite effacer mon compte ainsi que mes annonces, conversations, messages et autres informations liées ?</h5>';
        alerte +=         '<div id="ouiOuNon">';
        alerte +=             '<button id="buttonOui" class="supprimer-annonce-oui" name="log-btn">oui</button>';
        alerte +=             '<button id="buttonNon" class="supprimer-annonce-non" name="log-btn">non</button>';
        alerte +=         '</div>';
        alerte +=     '</div>';

        $('#login').remove();
        $("body").append(alerte);
        $("body").append('<script src="js/messageClose.js"></script>');
        $("body").append('<script src="js/effacerCompte2.js"></script>');
    })
})