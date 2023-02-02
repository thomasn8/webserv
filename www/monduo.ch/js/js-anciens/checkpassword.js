$('#mdp').on('input', function(){  //check si le password répond aux conditions regEx
    var conditions = '&nbsp;&#x2718; Ton mot de passe doit contenir au moins 6 caractères dont 1 chiffre';
    var valide = '&nbsp;&#x2714; Mot de passe valable'
    $input = $('#mdp').val();
    if($input.length < 6) { 
        $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if(!/[a-z]/.test($input) || !/[0-9]/.test($input)) { 
      $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
    }
    else if(/[^a-zA-Z0-9._-]/.test($input)) { 
        $('#regex2').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
      }
    else { 
        $('#regex2').html(valide).removeClass().addClass('available').hide().fadeIn(400);
    }
  })