$(function() {
    $(".supprimer-annonce-oui").on('click', function() {

        var conversationNO = $('div.conversationNO').attr('id');

        var conversationDelete = { "conversationDelete": conversationNO };

        $.ajax({
            type: "POST", 
            url: "supprimerConversation.php",
            data: conversationDelete,
            success: function() {
                location.reload();
            }
        });
    })

    $(".supprimer-annonce-non").on('click', function() {
        $('#login').remove();
    })
})