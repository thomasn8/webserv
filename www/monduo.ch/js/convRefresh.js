$(function() {

    var conversationNO = $('#inputConversationNO').val();
    var contact = $('#inputReceveur').val();

    var dataConversation = { "conversationNO": conversationNO, "contact": contact, "limit": 'limit20' };
    var dataConversation2 = { "conversationNO": conversationNO, "contact": contact, "limit": 'limit40' };
    var dataConversation3 = { "conversationNO": conversationNO, "contact": contact, "limit": 'limit100' };

    if($('div#login.chat') !== null) {
        function timeout() {
            setTimeout(function () {

                var scrollTop = $('div.chatZone').scrollTop();
                var limit = $("div.chatZone").children().length;


                if (((scrollTop <= 0) && (limit >= 40)) || (limit > 40) ){
                    $.ajax({
                        type: "POST", 
                        url: "chat.php",
                        data: dataConversation3,
                        success: function(response) {
                            $(".chatZone").html(response);
                            
                            // si le scroll est tout en bas dans la conv (vue sur les derniers msg) => le refresh ramène le scroll en bas du nouveau msg
                            if($('div.chatZone').scrollTop() + $('div.chatZone').innerHeight() >= $('div.chatZone')[0].scrollHeight) {
                                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                            }
                            // ... permet à l'utilisateur de regarder des messages plus anciens (en haut) sans être scrollé auto en bas lors des new msg
                        },
                        fail: function() {
                            $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                            $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                        }
                    });
                }
                else if ((scrollTop <= 0) && (limit = 20)) {
                    $.ajax({
                        type: "POST", 
                        url: "chat.php",
                        data: dataConversation2,
                        success: function(response) {
                            $(".chatZone").html(response);
                            
                            // si le scroll est tout en bas dans la conv (vue sur les derniers msg) => le refresh ramène le scroll en bas du nouveau msg
                            if($('div.chatZone').scrollTop() + $('div.chatZone').innerHeight() >= $('div.chatZone')[0].scrollHeight) {
                                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                            }
                            // ... permet à l'utilisateur de regarder des messages plus anciens (en haut) sans être scrollé auto en bas lors des new msg
                        },
                        fail: function() {
                            $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                            $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                        }
                    });
                }
                else if ((limit > 20) && (limit <= 40)){
                    $.ajax({
                        type: "POST", 
                        url: "chat.php",
                        data: dataConversation2,
                        success: function(response) {
                            $(".chatZone").html(response);
                            
                            // si le scroll est tout en bas dans la conv (vue sur les derniers msg) => le refresh ramène le scroll en bas du nouveau msg
                            if($('div.chatZone').scrollTop() + $('div.chatZone').innerHeight() >= $('div.chatZone')[0].scrollHeight) {
                                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                            }
                            // ... permet à l'utilisateur de regarder des messages plus anciens (en haut) sans être scrollé auto en bas lors des new msg
                        },
                        fail: function() {
                            $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                            $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                        }
                    });
                }
                else {
                    $.ajax({
                        type: "POST", 
                        url: "chat.php",
                        data: dataConversation,
                        success: function(response) {
                            $(".chatZone").html(response);
                            
                            // si le scroll est tout en bas dans la conv (vue sur les derniers msg) => le refresh ramène le scroll en bas du nouveau msg
                            if($('div.chatZone').scrollTop() + $('div.chatZone').innerHeight() >= $('div.chatZone')[0].scrollHeight) {
                                $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                            }
                            // ... permet à l'utilisateur de regarder des messages plus anciens (en haut) sans être scrollé auto en bas lors des new msg
                        },
                        fail: function() {
                            $(".chatZone").html('Problème lors du chargement de la conversation, désolé. Essaie de te reconnecter.');
                            $('div.chatZone').scrollTop($('div.chatZone').prop("scrollHeight"));
                        }
                    });
                }

                timeout();
            }, 3000);
        }
        timeout();
    }
})