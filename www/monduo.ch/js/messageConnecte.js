    function fermerMessage() {
        $('#login').remove();
    }
    var fermer = document.getElementById('close');
    fermer.addEventListener('click', fermerMessage, false);
    function changeDisplay() {
        $('#login').remove()
    }

    setTimeout(function(){ location.reload() }, 1200); //timeout avant le reload