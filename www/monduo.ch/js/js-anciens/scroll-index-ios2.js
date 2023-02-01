$(window).scroll(function() {
    if($(window).width() < 1000) {
        if($(window).scrollTop() > 800) {
            $('#scrollMessage').css('display', 'block');
            $('#scrollMessage').animate({'opacity': '0.5'}, 250);
        }
        else {
            $('#scrollMessage').stop(true).animate({
                'opacity': '0'}, 250, function() { $(this).css('display', 'none') });
        }
    }
})

function topFunction() {                        //ajouter   "   onclick="topFunction()"   "   sur la div du bouton
    document.body.scrollTop = 0; // For Safari
    document.documentElement.scrollTop = 0; // For Chrome, Firefox, IE and Opera
} 