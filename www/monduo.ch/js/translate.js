$(document).ready(function(){
    if($(window).width() > 599) {
        setTimeout(function() {
            $('div.forme-encadre1').addClass('encadre1-translate');
            $('div.forme-encadre2').addClass('encadre2-translate');
        }, 300);
            
        setTimeout(function() {
            $('div.forme-encadre1').addClass('encadre1-detranslate');
            $('div.forme-encadre2').addClass('encadre2-detranslate');
        }, 800);
    }/*
    else {
        setTimeout(function() {
            $('div.forme-encadre1').addClass('encadre1-verti');
            $('div.forme-encadre2').addClass('encadre2-verti');
        }, 300);
            
        setTimeout(function() {
            $('div.forme-encadre1').addClass('encadre1-deverti');
            $('div.forme-encadre2').addClass('encadre2-deverti');
        }, 800);
    }*/
})

/*
//Version à la 1ère visite seulement
$(document).ready(function(){
    if(!localStorage["alertdisplayed"]) {

        //code ici

        localStorage["alertdisplayed"] = true
    }
})
*/