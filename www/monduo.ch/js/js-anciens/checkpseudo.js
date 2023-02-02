$('#user').on('input', function()     //check si le pseudo est pris/libre
{
  pseudo = this
  if (pseudo.value == '')
  {
    $('#used2').html('')
    return
  }
  else{
      $.post
      (
        'checkpseudo.php',
        { pseudo : pseudo.value },
        function(data)
        {
          $('#used2').html(data).hide().fadeIn(400);
        }
      )
  }
})

$('#user').on('input', function(){  //check si le pseudo répond aux conditions regEx
  var conditions = '&nbsp;&#x2718; Ton pseudo doit contenir au moins 6 caractères (caractères spéciaux autorisés : .-_ )';
  var valide = '&nbsp;&#x2714; Pseudo valable'
  $input = $('#user').val();
  if($input.length < 6) { 
      $('#regex1').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
  }
  else if(/[^a-zA-Z0-9._-]/.test($input)) { 
    $('#regex1').html(conditions).removeClass().addClass('taken').hide().fadeIn(400);
  }
  else { 
      $('#regex1').html(valide).removeClass().addClass('available').hide().fadeIn(400);
  }
})