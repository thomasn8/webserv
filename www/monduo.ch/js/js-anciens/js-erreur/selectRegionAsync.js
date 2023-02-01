$(function() {
                            
    $("#domaineID").bind("change", function() {
        $.ajax({
            type: "GET", 
            url: "selectMatiere.php",
            data: "domaineID="+$("#domaineID").val(),
            success: function(html) {
                $("#categID").html(html);
            }
        });
    });
                
    
    });