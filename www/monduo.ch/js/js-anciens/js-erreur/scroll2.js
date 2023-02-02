$(window).scroll(function() {
    if($(window).width() < 1000) {
        if($(window).scrollTop() > 800) {
            $('#scrollMessage').css('display', 'block');
            $('#scrollMessage').animate({'opacity': '0.5'}, 250);
            $('#scrollMessage').on('click', function(event) {

                if (navigator.userAgent.match(/(iPod|iPhone|iPad|Android)/)) {           
                    $(document).animate({ scrollTop: $(document).offset().top }, 500)
                }
                else{
                event.preventDefault();
                $('html, body').stop().animate({ scrollTop: $('html, body').offset().top }, 500)
                }
            });  
        }
        else {
            $('#scrollMessage').stop(true).animate({
                'opacity': '0'}, 250, function() { $(this).css('display', 'none') });
        }
    }
})