$("#publish2").submit(function(event) {

    event.preventDefault();

    //sélectionner   (' .title h5 span ')  et lui append() ou html()  <?php $error ?>
    var errorMsg = '<?php $error ?>';
    $("#errorMsg").html(errorMsg) ;
});