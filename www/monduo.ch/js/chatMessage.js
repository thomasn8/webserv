$(function() {
    $("button#sendMessage").on('click', function(event) {

        event.preventDefault();

        var msg = { "receveur": $("#inputReceveur").val(), "conversationNO": $("#inputConversationNO").val(),
        "annonceNO": $("#inputAnnonceNO").val(), "message": $("#message").val() };

        var message = $("#message").val();

        if(message !== ''){
            $.ajax({
                type: "POST", 
                url: "chatMessage.php",
                data: msg,
                success: function(messageSent) {
                    $("textarea#message").val('');
                    $("div.chatZone").children().first().before(messageSent);
                    //Pour que le scroll du chat se mette au fond :
                    //Sans animation : $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                    $('div.chatZone').animate({ scrollTop: $('div.chatZone').prop("scrollHeight")}, 300);
                },
                fail: function() {
                    $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                    $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                }
            });
        }
    });
})