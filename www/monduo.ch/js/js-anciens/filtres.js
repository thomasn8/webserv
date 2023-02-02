$(function() {
    $(".filtres").on('change', function() {
        var valeurs = { "domaineID": $("#domaineID").val(), "matiereID": $("#matiereID").val(), "regionID": $("#regionID").val()};
    
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

                //test pour régler problème de annonce_contact / connectToContact après avoir sélectionné un filtre
                //MAINTENANT AVEC AJAX FAIRE QUE SELON SI UNE SESSION EST ACTIVE OU NON, body.append js/connectToContact ou js/annonce_contact
                $("body").append('<script src="js/connectToContact.js"></script>');
                $("body").append('<script src="js/annonce_contact.js"></script>');
            },
            fail: function(){
                var messageErreur = '<tr>Désolé, nous rencontrons un problème d\'affichage des annonces. Relance ta recherche ou recharge la page</tr>';
                titresSiblings.remove();
                titres.after(messageErreur);
            },
        });
    })
    $("#domaineID").on('change', function() {
        if($("#matiereID").val() !== 'allMatiere') { //clause particulière
            var valeurs = { "domaineID": $("#domaineID").val(), "matiereID": 'allMatiere', "regionID": $("#regionID").val()};
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

                    //test pour régler problème de annonce_contact / connectToContact après avoir sélectionné un filtre
                    //MAINTENANT AVEC AJAX FAIRE QUE SELON SI UNE SESSION EST ACTIVE OU NON, body.append js/connectToContact ou js/annonce_contact
                    $("body").append('<script src="js/connectToContact.js"></script>');
                    $("body").append('<script src="js/annonce_contact.js"></script>');
                },
                fail: function(){
                    var messageErreur = '<tr>Problème d\'affichage des annonces. Relance ta recherche ou recharge la page</tr>';
                    titresSiblings.remove();
                    titres.after(messageErreur);
                },
            });
        }
    })
})

//rajouter un event qui au refresh de la page => sélectionne les filtres select Domaine et Région et les remette à 0 (option tous/toutes)

//!!!!!!!sélectionner l'event refresh (NE MARCHE PAS!!!)
//$(window).bind('beforeunload', function() { alert('coucou') } );

//selecteurs et méthodes possibles pour Region:
//$('select#regionID option:first').attr('selected')
//$('select#regionID:first-child').attr('selected')
//$('#regionID:first-child').attr('selected') 