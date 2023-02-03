image = O('looper')                         //définir l'ID de l'image comme ici dans O('...')
//lier au code du fichier OSC.js pour que ça fonctionne

src1 = 'images/pub11.png'                      //taper le chemin de la 1ère img ici (celle qui est dans le html)
src2 = 'images/pub21.png'                     //taper le chemin de la 2ème img ici

setTimeout(setPicture2, 5000)

function setPicture2() {
    image.src = src2
    setTimeout(setPicture1, 5000)

    function setPicture1() {
        image.src = src1
        setTimeout(setPicture2, 5000)
    }
}
