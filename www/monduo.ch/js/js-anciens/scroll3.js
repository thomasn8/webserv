$(window).scroll(function() {
    if($(window).width() < 1000) {
        if($(window).scrollTop() > 800) {
            $('#scrollMessage').css('display', 'block');
            $('#scrollMessage').animate({'opacity': '0.5'}, 250);
            $('a[href^="#"]').on('click', function(event) {
                var target = $(this.getAttribute('href'));
                    event.preventDefault();
                    $('html, body').stop().animate({ scrollTop: target.offset().top }, 500);                                                      
            });  
        }
        else {
            $('#scrollMessage').stop(true).animate({
                'opacity': '0'}, 250, function() { $(this).css('display', 'none') });
        }
    }
})