#include "parse_config.hpp"

void exitWithError(std::ostream & stream, const std::string message, const std::string line, int code)
{
	stream << message << line << std::endl;
	exit(code);
}

int tabLength(std::string *tab)
{
	int i = 0;
	while (!tab[i].empty())
		i++;
	return i;
}

std::string TrimFunction(std::string str)
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0,str.find_first_not_of(typeOfWhitespaces));
	return str;
}

bool isNotBlank(std::string line)
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

bool isNotAlphaNum(std::string line)
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

bool prefixSyntax(std::string prefix)
{
	// check debut du prefix: commence par / OU par . OU *.
	if (!(prefix[0] == '/' || prefix[0] == '.' || (prefix[0] == '*' && prefix[1] == '.')))
		return false;
	
	// check format extension: que des alpha-num apres le . OU le *.
	if (prefix[0] == '.')
		prefix.erase(0,1);
	else if ((prefix[0] == '*' && prefix[1] == '.'))
		prefix.erase(0,2);
	if (prefix[0] != '/' && isNotAlphaNum(prefix))
		return false;
	
	// 1 seul mot (donc pas de whitespace, puisque prefix a deja ete trime)
	if (!isNotBlank(prefix))
		return false;
	return true;
}
