#include "parse_config.hpp"

void p_exit_cerr_msg(const std::string message, const std::string line, int code)
{
	std::cerr << message << line << std::endl;
	exit(code);
}

int p_tablen(const std::string *tab)
{
	int i = 0;
	while (!tab[i].empty())
		i++;
	return i;
}

std::string p_trim_sides(std::string str)
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0,str.find_first_not_of(typeOfWhitespaces));
	return str;
}

bool p_isnotblank(std::string line)
{
	std::string::iterator it = line.begin();
	std::string::iterator ite = line.end();
	while (it != ite)
	{
		if (!isblank(*it))
			return true;
		it++;
	}
	return false;
}

bool p_isnotalphanum(std::string line)
{
	std::string::iterator it = line.begin();
	std::string::iterator ite = line.end();
	while (it != ite)
	{
		if (!isalnum(*it))
			return true;
		it++;
	}
	return false;
}

bool p_prefix_syntax(std::string prefix)
{
	// check debut du prefix: commence par / OU par . OU *.
	if (!(prefix[0] == '/' || prefix[0] == '.' || (prefix[0] == '*' && prefix[1] == '.')))
		return false;
	
	// check format extension: que des alpha-num apres le . OU le *.
	if (prefix[0] == '.')
		prefix.erase(0,1);
	else if ((prefix[0] == '*' && prefix[1] == '.'))
		prefix.erase(0,2);
	if (prefix[0] != '/' && p_isnotalphanum(prefix))
		return false;
	
	// 1 seul mot (donc pas de whitespace, puisque prefix a deja ete trime)
	if (!p_isnotblank(prefix))
		return false;
	return true;
}

bool p_error_page_syntax(std::string line, std::string word)
{
	return true;
}

bool p_method_syntax(std::string word)
{
	return true;
}

// line doit avoir soit
// 2 str + 1 code
// 2 str
// 1 str + 1 code
// le code peut etre a n'importe quelle position
bool p_redirect_syntax(std::string line, std::string word)
{
	return true;
}
