$(function() {
    if($("#domaineID").val() == 'allDomaines' ) {
        $.ajax({
            type: "GET", 
            url: "selectMatiere.php",
            data: "domaineID="+$("#domaineID").val(),
            success: function(html) {
                $("#categID").html( '<option value="allCateg" selected="selected">Toutes</option>' + html);
            }
        });
    }
})