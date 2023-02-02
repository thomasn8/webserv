$(function() {
    $(".filtres").on('change', function() {
        var valeurs = { "role": $("#role").val(), "domaineID": $("#domaineID").val(), "matiereID": $("#matiereID").val(), "regionID": $("#regionID").val(), "limit": $(".filtre-limit:checked").val()};

        $.ajax({
            type: "GET", 
            url: "filtres.php",
            data: valeurs,
            timeout: 2000,
            success: function(html) {

                var titres = $('#th');
                var titresSiblings = titres.nextAll();

                titresSiblings.remove();
                titres.after(html).hide().fadeIn(400);
                $("body").append('<script src="js/annonce_contact.js"></script>');
                $("body").append('<script src="js/profile-hover.js"></script>');
            },
            fail: function(){
                var messageErreur = '<tr>Erreur lors de la récupération des annonces auprès du serveur. Essaie d\'actualiser cette page ou de relancer ton navigateur. Désolé pour ce contretemps</tr>';
                titresSiblings.remove();
                titres.after(messageErreur);
            },
        });
    })
    $("#domaineID").on('change', function() {
        if($("#matiereID").val() !== 'allMatiere') { // = condition particulière pour contrôller le lien entre les filtres domaine et matière
            var valeurs = { "role": $("#role").val(), "domaineID": $("#domaineID").val(), "matiereID": 'allMatiere', "regionID": $("#regionID").val(), "limit": $(".filtre-limit:checked").val()};
            //si la matiere n'est pas égal à allMatiere lors du changement de domaine => remet la valeur de matiere à allMatiere

            $.ajax({
                type: "GET", 
                url: "filtres.php",
                data: valeurs,
                timeout: 2000,
                success: function(html) {
    
                    var titres = $('#th');
                    var titresSiblings = titres.nextAll();
    
                    titresSiblings.remove();
                    titres.after(html).hide().fadeIn(400);

                    $("body").append('<script src="js/connectToContact.js"></script>');
                    $("body").append('<script src="js/annonce_contact.js"></script>');
                    $("body").append('<script src="js/profile-hover.js"></script>');
                },
                fail: function(){
                    var messageErreur = '<tr>Erreur lors de la récupération des annonces auprès du serveur. Essaie d\'actualiser cette page ou de relancer ton navigateur. Désolé pour ce contretemps</tr>';
                    titresSiblings.remove();
                    titres.after(messageErreur);
                },
            });
        }
    })
})