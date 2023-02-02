$(function() {
//AU CHARGEMENT de la page, affiche toutes les matieres dans le select menu (sauf les catégories Autres de chaque matière)
    $.ajax({
        type: "GET", 
        url: "selectMatiereVoir.php",
        data: "domaineID="+$("#domaineID").val(),
        success: function(html) {
            $("#matiereID").html('<option value="allMatiere" selected="selected">Toutes</option>' + html);                
        }
    });
//LORSQUE L'USER CHANGE LA VALEUR du select menu Domaines, affiche les matieres correspondantes                          
    $("#domaineID").bind("change", function() {
        $.ajax({
            type: "GET", 
            url: "selectMatiereVoir.php",
            data: "domaineID="+$("#domaineID").val(),
            success: function(html) {
                    $("#matiereID").html('<option value="allMatiere" selected="selected">Toutes</option>' + html);
            }
        });
    }); 
});