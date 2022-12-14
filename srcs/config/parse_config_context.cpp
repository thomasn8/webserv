#include "parse_config.hpp"

int open_server_block(std::string & line, bool *server_context, int *server_count)
{
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
		if (!isblank(*it))
			return INVALID;
	}
	return VALID;
}

int open_server_block_2(std::string & line, bool *server_context, int *server_count)
{
	(void) server_count;
	if (*server_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("{");
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
