$(function() {
    function fermerMessage() {
        $('#login').remove();
        location.reload();
    }
    var fermer = document.getElementById('close');
    fermer.addEventListener('click', fermerMessage, false);
})