screenSiz = window.innerWidth

image = C('looper')

src1 = 'images/pub1.png'
src2 = 'images/pub3.png'

src7 = 'images/pub2.png'
src8 = 'images/pub4.png'


if (screenSiz < 500) {
    setTimeout(setPicture2phone, 1000)
}
else if (screenSiz < 1000) {
    setTimeout(setPicture2tablet, 1000)
} else {
    setTimeout(setPicture2, 1000)
}


function setPicture2phone() {
    image.srcset = src2
    setTimeout(setPicture1phone, 1000)

    function setPicture1phone() {
        image.srcset = src1
        setTimeout(setPicture2phone, 1000)
    }
}


function setPicture2tablet() {
    image.srcset = src7
    setTimeout(setPicture1tablet, 1000)

    function setPicture1tablet() {
        image.srcset = src8
        setTimeout(setPicture2tablet, 1000)
    }
}

function setPicture2() {
    image.src = src2
    setTimeout(setPicture1, 1000)

    function setPicture1() {
        image.src = src1
        setTimeout(setPicture2, 1000)
    }
}