$(function() {
    $(".contactButton").on('click', function() {

        var ligne = $(this).parent().parent();
        var qui   = ligne.find(".profile-button").html();
        var quoi  = ligne.find(".discipline").html();
        var annonceNO = ligne.find("button").attr('id');



        var contact =   '<div id="login">';
        contact +=          '<span id="close"></span>';
        contact +=          '<div id="' + annonceNO + '" class="title subject">' + quoi + ' :<br>écrire @<span id="annonceur">' + qui + '</span></div>';
        contact +=          '<form id="contact" action="voir.php" method="post">';
        contact +=              '<textarea id="messageTo" name="message" maxlength="800" placeholder="Ton message..." required></textarea>';
        contact +=              '<input type="hidden" name="annonceur" value="' + qui + '" />';
        contact +=              '<input type="hidden" name="annonceNO" value="' + annonceNO + '" />';
        contact +=              '<input type="hidden" name="matiere" value="' + quoi + '" />';
        contact +=              '<button type="submit" id="sendMessage" name="sendMessage">Envoyer</button>';
        contact +=          '</form>';
        contact +=      '</div>';
    
        $('#login').remove();
        $("body").append(contact);
        $("body").append('<script src="js/messageClose.js"></script>');
    })
})