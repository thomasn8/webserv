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

$(document).ready(function(){
    $('#scrollMessage').on('click', function(event) {
        event.preventDefault();
        $('html, body').stop().animate({ scrollTop: 0 }, 500);
        return false;
    });
})