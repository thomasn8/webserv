$(function() {
    $("#deconnexion").on('click', function(event) {
        
        event.preventDefault();
        var userPseudo = $("span#userPseudo").html();
        var ajax = { "userPseudo": userPseudo };

        $.ajax({
            type: "POST", 
            url: "deconnexion.php",
            data: ajax,
            success: function() {
                location.reload()
            }
        });
    })
})