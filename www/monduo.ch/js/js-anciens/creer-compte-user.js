function changeClass() {                                    //ajoute les indications concernant la création d'un nom d'utilisateur
    var contrainte1 = document.getElementById('p1');
    contrainte1.setAttribute('class', 'contrainte1');
}
var user = document.getElementById('user');
user.addEventListener('focus', changeClass, false);




function removeClass() {                                    //enlève les indications concernant la création d'un nom d'utilisateur
    var contrainte1 = document.getElementById('p1');
    contrainte1.setAttribute('class', '');
}

user.addEventListener('blur', removeClass, false);