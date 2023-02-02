$(function() {
    $("#sendMessage").on('click', function(event) {

        //event.preventDefault();

        var annonceur  = $('#annonceur').html();
        var annonceNO  = $('.subject').attr('id');
    
        var infos = { "annonceur": annonceur, "annonceNO": annonceNO};
    
        $.ajax({
            type: "GET", 
            url: "voir.php",
            data: infos,
        });
    })
})