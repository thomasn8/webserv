$(function() {
    function fermerMessage() {
        $('#login').remove();
        window.location.replace("https://monDUO.ch/recup-mdp.php");
    }
    var fermer = document.getElementById('close');
    fermer.addEventListener('click', fermerMessage, false);
})