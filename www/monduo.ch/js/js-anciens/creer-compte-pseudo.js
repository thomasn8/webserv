$('#user').blur(function() { $('#p1').hide('fast', 'swing') })
$('#user').focus(function() { $('#p1').show('fast', 'swing') })

$('#mdp').blur(function() { $('#p2').hide('fast', 'swing') })
$('#mdp').focus(function() { $('#p2').show('fast', 'swing') })

//<p id="p1" class="">6 caractères minimum (chiffres et majuscules autorisés ainsi que les caractères: _ -)</p>
//<p id="p2" class="">6 caractères minimum et au moins 1 chiffre</p>
