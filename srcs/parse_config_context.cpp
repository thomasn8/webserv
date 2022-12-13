#include "parse_config.hpp"

int open_server_block(std::string & line, std::string & prevWord, bool *server_context, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("server");
	std::string::iterator it(&line[pos + 5]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it) && (*it) != '{')
			return INVALID;
		if (*it == '{')
			*server_context = true;
	}
	return VALID;
}

int open_server_block_2(std::string & line, std::string & prevWord, bool *server_context, int *server_count)
{
	(void) server_count;
	if (*server_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("{");
	if (prevWord.compare(0, std::string::npos, "server", prevWord.length()) != EQUAL)
	{
		line = prevWord;
		return INVALID;
	}
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_context = true;
	return VALID;
}

int open_server_block_3(std::string & line, std::string & prevWord, bool *server_context, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("server{");
	std::string::iterator it(&line[pos + 6]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_context = true;
	return VALID;
}