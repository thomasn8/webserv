$(function() {
    $("a.profile-button").on('click', function(event) {
        
        event.preventDefault();

        var ligne = $(this).parent().parent();
        var qui = $(this).attr('id');
        //var quoi  = ligne.find(".discipline").html();

        var profile =   '<div id="login" class="profiles">';
        profile +=          '<span id="close" class="closeProfile"></span>';
        profile +=          '<div class="title subject user">' + qui + '</div>';
        profile +=          '<div id="view-profile"></div>';
        profile +=      '</div>';
    
        $('#login').remove();
        $("body").append(profile);
        $("body").append('<script src="js/messageClose.js"></script>');


        var profilePseudo = { "Pseudo": qui };

        $.ajax({
            type: "POST", 
            url: "description.php",
            data: profilePseudo,
            success: function(description) {
                $("#view-profile").html(description);
            }
        });
    })
})