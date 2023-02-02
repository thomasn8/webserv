$(function() {
    $(".deleteConv").on('click', function(event) {
        
        event.preventDefault();

        var conversationNO = $(this).attr('id');
        var contact = $(this).parent().attr('class');
        
        var alerte =  '<div id="login">';
        alerte +=         '<span id="close"></span>';
        alerte +=         '<div id="'+conversationNO+'" class="title conversationNO">Supprimer cette conversation ?<span class="precision">(pareillement, elle n\'apparaîtra plus chez <span>'+contact+'</span>)</span></div>';
        alerte +=         '<div id="ouiOuNon">';
        alerte +=             '<button class="supprimer-annonce-oui" name="log-btn">Oui</button>';
        alerte +=             '<button class="supprimer-annonce-non" name="log-btn">Non</button>';
        alerte +=         '</div>';
        alerte +=     '</div>';

        $('#login').remove();
        $("body").append(alerte);
        $("body").append('<script src="js/messageClose.js"></script>');
        $("body").append('<script src="js/supprimerConversation2.js"></script>');
    })
})