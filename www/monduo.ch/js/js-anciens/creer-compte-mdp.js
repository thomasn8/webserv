function changeClass() {                                    //ajoute les indications concernant la création d'un mdp
    var contrainte2 = document.getElementById('p2');
    contrainte2.setAttribute('class', 'contrainte2');
}
var user = document.getElementById('mdp');
user.addEventListener('focus', changeClass, false);




function removeClass() {                                    //enlève les indications concernant la création d'un mdp
    var contrainte2 = document.getElementById('p2');
    contrainte2.setAttribute('class', '');
}

user.addEventListener('blur', removeClass, false);