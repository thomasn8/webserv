$(function() {
    $("#log-btn").on('click', function() {

        var identifiants = { "identifiant1": $("#identifiant1").val().trim(), "identifiant2": $("#identifiant2").val().trim() };

        $.ajax({
            type: "POST", 
            url: "loginAndCreateSession.php",
            data: identifiants,
            success: function(response) {
                $("span#errorMsg").html(response).hide().fadeIn(400);;
                $("body").append('<script src="js/connexionValidation.js"></script>');
            }
        });
        return false;
    });
})