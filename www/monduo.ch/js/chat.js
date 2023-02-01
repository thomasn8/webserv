$(function() {
    $("a.conv").on('click', function(event) {
        
        event.preventDefault();

        var annonceNO = $(this).parent().attr('id');
        var conversationNO = $(this).attr('href');
        var contact = $(this).children('.contact').html();
        var discipline = $(this).children('.matiere').html();

        var dataConversation = { "conversationNO": conversationNO, "contact": contact  };


        var chat =   '<div id="login" class="chat">';
        chat +=          '<span id="close"></span>';
        chat +=          '<div class="title subject">' + contact + '<span id="showProfile">voir son profil</span></div>';
        chat +=          '<h5 class="discipline">' + discipline + '<span id="effacerConv" class="'+contact+'"><a id="'+conversationNO+'" class="deleteConv" href="#">supprimer la conversation</a></span></h5>';
        chat +=          '<div class="chatZone"></div>';
        chat +=          '<form id="chatMessage" action="" method="post">';
        chat +=              '<textarea id="message" name="message" maxlength="800" placeholder="Ton message ..."></textarea>';
        chat +=              '<input id="inputReceveur" type="hidden" name="receveur" value="'+contact+'" required/>';
        chat +=              '<input id="inputConversationNO" type="hidden" name="conversationNO" value="'+conversationNO+'" required/>';
        chat +=              '<input id="inputAnnonceNO" type="hidden" name="annonceNO" value="'+annonceNO+'" required/>';
        chat +=              '<button id="sendMessage" type="submit" name="sendMessage" value="1">Envoyer</button>';
        chat +=          '</form>';
        chat +=      '</div>';

    
        $('#login').remove();
        $("body").append(chat);
        $("body").append('<script src="js/messageClose2.js"></script>');
        $("body").append('<script src="js/chatMessage.js"></script>');
        $("body").append('<script src="js/convRefresh.js"></script>');
        $("body").append('<script src="js/supprimerConversation.js"></script>');
        $("body").append('<script src="js/profileChat.js"></script>');


        $.ajax({
            type: "POST", 
            url: "chat.php",
            data: dataConversation,
            success: function(response) {
                $(".chatZone").html(response);
                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
            },
            fail: function() {
                $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
            }
        });
    })
})