$(function() {
    function checkforText(requiredText) { 
        found = false; 

        $("span#errorMsg").each((id, elem) => { 
            if (elem.innerText == requiredText) { 
                found = true; 
            } 
        }); 

        return found; 
    } 

    test = checkforText('connexion');

    if (test == true) {
        setTimeout(function(){ $("span#errorMsg").append(' .') },500);
        setTimeout(function(){ $("span#errorMsg").append(' .') },1000);
        setTimeout(function(){ $("span#errorMsg").append(' .') },1500);
        setTimeout(function(){ location.reload() }, 2000); //timeout avant le reload
    }
})