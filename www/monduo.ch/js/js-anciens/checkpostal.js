$('#postal').on('blur', function(){  //check si le code postal répond aux conditions regEx
    var conditions = '&nbsp;&#x2718; Code postal invalide';

    $input = $('#postal').val();

    if(!/^[1-9]\d{3}$/.test($input)) { 
        $('#regex4').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else {
        $('#regex4').html('').removeClass()
    }
  })