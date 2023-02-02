$(function() {
    $("button.supprimer-annonce-oui").on('click', function() {
        var annonceNO = $('div.annonceNO').attr('id');

        var annonceDelete = { "annonceDelete": annonceNO };

        $.ajax({
            type: "POST", 
            url: "supprimer-prolonger-monAnnonce.php",
            data: annonceDelete,
            success: function() {
                location.reload();
            }
        });
    })

    $("button.supprimer-annonce-non").on('click', function() {
        $('#login').remove();
    })
})