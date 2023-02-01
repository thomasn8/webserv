$(function() {
    function checkforText(requiredText) { 
        found = false; 

        $("span#errorMsg").each((id, elem) => { 
            if (elem.innerText == requiredText) { 
                found = true; 
            } 
        }); 

        return found; 
    } 

    test = checkforText('connexion...');

    if (test == true) {
        $("#login").html('<span id="close"></span>Connecté-e<script src="js/messageConnecte.js"></script>');
        //bloquer tous les boutons de la page en cours
        $(".contactButton").prop('disabled', true);
    }
})