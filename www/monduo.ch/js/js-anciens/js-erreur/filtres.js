$(function() {      //POUR FAIRE TOURNER => remettre la classe .filtres au select matiere et changer le nom du js en filtres0.js
    $(".filtres").on('change', function() {

        var valeurs = { "domaineID": $("#domaineID").val(), "matiereID": $("#matiereID").val(), "regionID": $("#regionID").val()};

        $.ajax({
            type: "GET", 
            url: "filtres.php",
            data: valeurs,
            success: function(html) {

                var titres = $('#th');
                var titresSiblings = titres.nextAll();

                titresSiblings.remove();
                titres.after(html).hide().fadeIn(400);
            }
        });

        /* BUGG A CORRIGER :
        quand la matiere est selected (id2)
        et qu'ensuite le domaine est selected (id1) MAIS ne correspond pas à la matière (id2) => IMPOSSIBLE de trouver des données (normal)

        CORRECTION : 
        soit modifier le php pour modifier le cas (elseif) correspondant

        soit modifier la répondse du script js :
        quand l'utilisateur change la valeur de domaineID,
        si domaineID value == allDomaine
        AND
        si matiereID value !== allMatiere
        ==> LANCER une autre requête php qui irait chercher uniquement dans domaine :
        $result = queryMysql("SELECT matiere, resume, pseudo, region, niveau, partenaire FROM annonces WHERE domaineID = '$id1' ORDER BY date");
        selectAndReturnData($result);
        */

    });    
});