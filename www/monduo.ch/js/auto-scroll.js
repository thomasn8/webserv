$('a[href^="#"]').on('click', function(event) {
    var target = $(this.getAttribute('href'));
    if( target.length ) {
        event.preventDefault();
        $('html, body').stop().animate({
            scrollTop: target.offset().top - 100                    //100 étant la distance négative du scroll en px
        }, 500);                                                    //500 étant le temps du scroll
    }
});