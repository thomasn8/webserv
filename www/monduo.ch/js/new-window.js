$(function() {
    $("a#newWindow").on('click', function(event) {
        
        event.preventDefault();
        event.stopPropagation();
        window.open("https://www.caricactus.ch",'_blank');
    })

    $("a.new-fb").on('click', function(event) {
        
        event.preventDefault();
        event.stopPropagation();
        window.open("https://www.facebook.com/Monduoch-653782461899658/",'_blank');
    })

    $("a.new-lk").on('click', function(event) {
        
        event.preventDefault();
        event.stopPropagation();
        window.open("https://www.linkedin.com/company/monduo-ch/",'_blank');
    })
})