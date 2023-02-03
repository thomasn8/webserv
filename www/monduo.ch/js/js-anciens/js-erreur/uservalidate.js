formulaire = document.getElementById("formu");

      function validate(formulaire)
      {
        fail += validateUsername(formulaire.username.value)
        fail += validatePassword(formulaire.password.value)
        fail += validateEmail(formulaire.email.value)

    if   (fail == "")   return true
    else { alert(fail); return false }
    }

function validateUsername(field)
    {
    if (field == "") return "No Username was entered.\n"
    else if (field.length < 6)
        return "Usernames must be at least 6 characters.\n"
    else if (/[^a-zA-Z0-9_-]/.test(field))
        return "Only a-z, A-Z, 0-9, - and _ allowed in Usernames.\n"
    return ""
    }

function validatePassword(field)
    {
    if (field == "") return "No Password was entered.\n"
    else if (field.length < 6)
        return "Passwords must be at least 6 characters.\n"
    else if (! /[a-z]/.test(field) ||
                ! /[0-9]/.test(field))
        return "Passwords require one each of a-z, and 0-9.\n"
    return ""
    }

    function validateEmail(field)
      {
        if (field == "") return "No Email was entered.\n"
          else if (!((field.indexOf(".") > 0) &&
                     (field.indexOf("@") > 0)) ||
                     /[^a-zA-Z0-9.@_-]/.test(field))
            return "The Email address is invalid.\n"
        return ""
      }