    function fermerMessage() {
        $('#login').remove();
    }
    var fermer = document.getElementById('close');
    fermer.addEventListener('click', fermerMessage, false);
    function changeDisplay() {
        $('#login').remove()
    }
    setTimeout(changeDisplay, 7000)