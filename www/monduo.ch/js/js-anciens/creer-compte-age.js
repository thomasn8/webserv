// FONCTION DES CHECKBOX COMME DES RADIOBUTTON
var $elements = $('.radiocheck');

// Allow only one input to be selected
$elements.filter(':checked:not(:last)').prop('checked', false);

// Get selected input
var selected = $elements.filter(':checked')[0] || {};

// Handle event
$('.signAge').on('click', 'input:checkbox', function(e) {
  if (e.currentTarget === selected) {
    e.preventDefault(); 
  } else {
    selected.checked = false;
    selected = e.currentTarget;
  }
});



// AJOUT ET SUPRESSION DES COORDONNEES PARENTALES :
$elements.on('change', function() {
    if($('#mineur').is(':checked')) {
        var $parental = $('<h5 class="parents">Coordonnées d\'un de tes parents (ou de ton représentant légal) :</h5>');
        var $inputCoordonnees = $('<div class="identifiants coordonneesParents"><label><span class="ponctuation">Prénom :</span></label><input type="text" name="prenom2" maxlength="40" form="newuser" required /></div><div class="identifiants coordonneesParents"><label><span class="ponctuation">Nom :</span></label><input type="text" name="nom2" maxlength="40" form="newuser" required /></div>')
        $('div.signAge').after($inputCoordonnees).after($parental).hide().fadeIn(600);
    }
    else {
        $('.parents').remove();
        $('.coordonneesParents').remove();
    }
})
