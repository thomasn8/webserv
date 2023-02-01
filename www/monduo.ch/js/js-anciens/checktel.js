$('#tel').on('blur', function(){  //check si le telephone répond aux conditions regEx
    var conditions = '&nbsp;&#x2718; Ton numéro sans espace ni caractères spéciaux';
    var conditions2 = '&nbsp;&#x2718; numéro invalide';

    $input = $('#tel').val();
    if(!$.isNumeric($input)) { 
      $('#regex3').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if($input.length < 10) { 
        $('#regex3').html(conditions2).removeClass().addClass('taken').hide().fadeIn(400);
      }
    else if( (/^(?!0).*/.test($input)) && (/^(?!4).*/.test($input)) ) { 
        $('#regex3').html(conditions2).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else {
        $('#regex3').html('').removeClass()
    }
  })