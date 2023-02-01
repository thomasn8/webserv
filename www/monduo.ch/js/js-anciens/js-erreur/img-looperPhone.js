image = O('publiciteLooperPhone')                         //définir l'ID de l'image comme ici dans O('...')
//lier au code du fichier OSC.js pour que ça fonctionne

src1 = 'images/pub1.png'                      //taper le chemin de la 1ère img ici (celle qui est dans le html)
src2 = 'images/pub3.png'                     //taper le chemin de la 2ème img ici

setTimeout(setPicture2, 1000)

function setPicture2() {
    image.srcset = src2
    setTimeout(setPicture1, 1000)

    function setPicture1() {
        image.srcset = src1
        setTimeout(setPicture2, 1000)
    }
}
