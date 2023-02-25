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
	std::string::iterator it = line.begin() + pos + 5;
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
	std::string::iterator it = line.begin() + pos;
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_context = true;
	return VALID;
}

int close_server_block(std::string & line, bool *server_context, int *server_count)
{
	if (*server_context == false)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it = line.begin() + pos;
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_context = false;
	(*server_count)++;
	return VALID;
}

int open_location_block(std::string & line, std::string & route, int *location_context, int *location_count)
{
	(void) location_count;
	if (*location_context == 1 || *location_context == 2)
	{
		line = ERROR_LOCATION_BLOCK;
		return INVALID;
	}
	int pos = line.find("location");
	if (!isblank(line[pos + 8]))
		return INVALID;
	route = p_trim_sides(line.substr(pos + 8));
	if (p_route_syntax(route) == false)
		return ROUTE_SYNTAX_INVALID;
	*location_context = 1;
	return VALID;
}

int open_location_block_2(std::string & line, std::string & route, int *location_context, int *location_count)
{
	(void) location_count;
	(void) route;
	if (*location_context != 1)
	{
		line = ERROR_LOCATION_BLOCK;
		return INVALID;
	}
	int pos = line.find("{");
	std::string::iterator it = line.begin() + pos;
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*location_context = 2;
	return VALID;
}

int close_location_block(std::string & line, std::string & route, int *location_context, int *location_count)
{
	(void) route;
	if (*location_context == 0)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it = line.begin() + pos;
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*location_context = 0;
	(*location_count)++;
	return VALID;
}
