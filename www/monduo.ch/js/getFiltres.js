$(function() {

    var queryString = window.location.search;
    var urlParams = new URLSearchParams(queryString);
    var selectedID = urlParams.get('id');

    //Si la page INDEX envoie une valeur ID de domaine 
    if(selectedID !== null) {
        var regex = /00$/;

        if(regex.test(selectedID) == true) {
            $('#domaineID option:selected').removeAttr('selected');
            $('[value= '+ selectedID +']').attr('selected', true);
            
            $.ajax({
                type: "GET", 
                url: "selectMatiereVoir.php",
                data: "domaineID="+$("#domaineID").val(),
                success: function(html) {
                    $("#matiereID").html('<option value="allMatiere" selected="selected">Toutes</option>' + html);                
                }
            });
        }
        //Si la page INDEX envoie une valeur ID de matiere
        else {

            domaineID = selectedID.substring(0,2).concat('00');
            $('#domaineID option:selected').removeAttr('selected');
            $('[value= '+ domaineID +']').attr('selected', true);

            $.ajax({
                type: "GET", 
                url: "selectMatiereVoir.php",
                data: "domaineID="+domaineID,
                success: function(html) {
                    $("#matiereID").html('<option value="allMatiere">Toutes</option>' + html);
                    $('#matiereID option:selected').removeAttr('selected');
                    $('[value= '+ selectedID +']').attr('selected', true);                
                }
            });
        }
    }
});