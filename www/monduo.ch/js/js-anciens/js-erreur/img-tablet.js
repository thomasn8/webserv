image = O('looper')

screenSiz = window.innerWidth

if (screenSiz > 500 && screenSiz < 1000) image.src = 'images/pub12.png'


source1 = 'images/pub12.png'
source2 = 'images/pub22.png'

setTimeout(setPicture2, 5000)

function setPicture2() {
    image.src = source2
    setTimeout(setPicture1, 5000)

    function setPicture1() {
        image.src = source1
        setTimeout(setPicture2, 5000)
    }
}