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

    test = checkforText('Publication de l\'annonce...');
    test2 = checkforText('Nombre maximum d\'annonce atteint');

    if (test == true) {
        $("button").prop('disabled', true);
        $("#login").html('<span id="close"></span>Ton annonce vient d\'être publiée. Tu peux la consulter et la modifier sur ton espace <a id="lienvers" href="mon-compte.php">Mon compte</a>.<br><br>Tu recevras une notification par email si quelqu\'un te répond.<script src="js/messageSignin.js"></script>');
    }
    else if (test2 == true) {
        $("button").prop('disabled', true);
        $("#login").html('<span id="close"></span>5 annonces maximum par utilisateur. Rends-toi sur ton espace <a id="lienvers" href="mon-compte.php">mon compte</a> pour gérer tes annonces.<script src="js/messageSignin.js"></script>');
    }
})