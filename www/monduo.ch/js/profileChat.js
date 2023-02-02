$(function() {
    $("span#showProfile").on('click', function() {
        
        var qui = $('input#inputReceveur').val();
        
        var profile =   '<div id="login" class="profiles">';
        profile +=          '<span id="close" class="closeProfile"></span>';
        profile +=          '<div class="title subject user">' + qui + '</div>';
        profile +=          '<div id="view-profile"></div>';
        profile +=      '</div>';
    
        $('#login').remove();
        $("body").append(profile);
        $("body").append('<script src="js/messageClose2.js"></script>');


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