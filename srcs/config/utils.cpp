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

bool p_route_syntax(std::string route)
{
	// check debut du route: commence par / OU par . OU *.
	if (!(route[0] == '/' || route[0] == '.' || (route[0] == '*' && route[1] == '.')))
		return false;
	
	// check format extension: que des alpha-num apres le . OU le *.
	if (route[0] == '.')
		route.erase(0,1);
	else if ((route[0] == '*' && route[1] == '.'))
		route.erase(0,2);
	if (route[0] != '/' && p_isnotalphanum(route))
		return false;
	
	// 1 seul mot (donc pas de whitespace, puisque route a deja ete trime)
	if (!p_isnotblank(route))
		return false;
	return true;
}

bool p_error_page_syntax(std::string line)
{
	static std::string prev;
	if (prev == line)
		return true;

	std::istringstream iss(line);
	std::string word;
	int code = 0, file = 0;
	iss >> word;
	while (iss)
	{
		if (iss.peek() == EOF)
			break;
		iss >> word;
		if (atoi(word.c_str()) == 0)
			file++;
		else
			code++;
	}
	prev = line;
	if (code < 1 || file != 1)
		return false;
	return true;
}

bool p_method_syntax(std::string word)
{
	if (word.compare("GET") != 0 && word.compare("POST") != 0 && word.compare("DELETE") != 0)
		return false;
	return true;
}

bool p_redirect_syntax(std::string line)
{
	static std::string prev;
	if (prev == line)
		return true;

	std::istringstream iss(line);
	std::string word;
	int words = 0, str = 0;
	iss >> word;
	while (iss)
	{
		if (iss.peek() == EOF)
			break;
		iss >> word;
		if (atoi(word.c_str()) == 0)
			str++;
		words++;
	}
	prev = line;
	// 2 mots: file, file/code
	// ou 3 mots, dont un num: file, file, code
	if (words < 2 || words > 3)
		return false;
	if (words == 2 && str < 1)
		return false;
	if (words == 3 && str < 2)
		return false;
	return true;
}
