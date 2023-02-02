$(function() {
    function fermerMessage() {
        $('#login').remove();
        window.location.replace("https://monDUO.ch/mon-compte.php");      
    }
    var fermer = document.getElementById('close');
    fermer.addEventListener('click', fermerMessage, false);
})