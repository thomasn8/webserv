#include "parse_config.hpp"

int check_open_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_block == true)
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
			*server_block = true;
	}
	return VALID;
}

int check_open_server_block_2(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) server_count;
	if (*server_block == true)
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
	*server_block = true;
	return VALID;
}

int check_open_server_block_3(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_block == true)
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
	*server_block = true;
	return VALID;
}

int check_close_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
	if (*server_block == false)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_block = false;
	(*server_count)++;	// DETERMINE LE NOMBRE D'OBJET CONFIG A AJOUTER AU SERVER
	std::cout << "\n";
	return VALID;
}
