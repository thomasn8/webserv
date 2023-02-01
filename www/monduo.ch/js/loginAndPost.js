$(function() {
    $("#button2").on('click', function() {

        var annonce = { "identifiant1": $("#identifiant1").val(), "identifiant2": $("#identifiant2").val(),
        "domaineID": $("#domaineID").val(), "matiereID": $("#matiereID").val(), 
        "resume": $("#resume").val().trim(), "regionID": $("#regionID").val(),
        "niveauID": $("#niveau").val(), "role": $(".role:checked").val(), "modalite": $(".modalite:checked").val() };

        $.ajax({
            type: "POST", 
            url: "loginAndPost.php",
            data: annonce,
            success: function(response) {
                $("span#errorMsg").html(response).hide().fadeIn(400);;
                $("body").append('<script src="js/validationAndBlockSubmitButton.js"></script>');
            }
        });
        return false;
    });
})