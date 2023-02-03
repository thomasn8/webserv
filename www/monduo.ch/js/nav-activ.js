jQuery(function($) {
    var path = window.location.href; // because the 'href' property of the DOM element is the absolute path
    $('.btn').each(function() {
     if (this.href === path) {
      $(this).addClass('active');
     }
    })
})

/*
//THIS WORKS TOO, MAKE A CHOICE
$(document).ready(function() {

    var CurrentUrl= document.URL;
    var CurrentUrlEnd = CurrentUrl.split('/').filter(Boolean).pop();
    console.log(CurrentUrlEnd);
    $( ".btn" ).each(function() {
          var ThisUrl = $(this).attr('href');
          var ThisUrlEnd = ThisUrl.split('/').filter(Boolean).pop();

          if(ThisUrlEnd == CurrentUrlEnd){
          $(this).closest('a').addClass('active')
          }
    });

});
*/

