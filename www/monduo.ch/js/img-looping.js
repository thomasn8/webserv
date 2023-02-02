image = O('looper') //sélectionne l'image de bannière
screenSiz = window.innerWidth //largeur d'écran
if (screenSiz > 500 && screenSiz < 1000) image.src = 'images/pub12.png' //pub12 = image au format rectangle
//si l'écran est de taille moyenne, choisit la bannière publicitaire au format rectangle




// !!! Pour modifier la durée d'affichage des bannières, changer partout le 2ème paramètre des Timeout !!!


//code pour la bannière au format carré :

src1 = 'images/pub11.png'   //image 1, bannière format carré
src2 = 'images/pub21.png'   //image 2, bannière format carré

if (screenSiz <= 500 || screenSiz >= 1000) {
    setTimeout(setPicture2, 5000)

    function setPicture2() {
        image.src = src2
        setTimeout(setPicture1, 5000)

        function setPicture1() {
            image.src = src1
            setTimeout(setPicture2, 5000)
        }
    }
}


//code pour la bannière au format rectangle :

source1 = 'images/pub12.png'    //image 1, bannière format rect.
source2 = 'images/pub22.png'    //image 2, bannière format rect.

if (screenSiz > 500 && screenSiz < 1000) {
    setTimeout(setPicture2, 5000)

    function setPicture2() {
        image.src = source2
        setTimeout(setPicture1, 5000)

        function setPicture1() {
            image.src = source1
            setTimeout(setPicture2, 5000)
        }
    }
}