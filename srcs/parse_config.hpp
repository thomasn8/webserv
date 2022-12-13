#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Server.hpp"
#include "Config.hpp"
#include "split_quotes.hpp"

# define INVALID 1
# define VALID 0
# define EQUAL 0
# define ERROR_MSG "Error: configuration file invalid:\n"
# define ERROR_SERVER_BLOCK "server{} directive error"

void parseConfig(std::string & configFile, Server & server);

typedef	int (*f_ptr)(std::string & line, std::string & prevWord, bool *x_block, int *x_count);

int open_server_block(std::string & line, std::string & prevWord, bool *server_context, int *server_count);
int open_server_block_2(std::string & line, std::string & prevWord, bool *server_context, int *server_count);
int open_server_block_3(std::string & line, std::string & prevWord, bool *server_context, int *server_count);
int close_server_block(std::string & line, std::string & prevWord, bool *server_context, int *server_count);

// int open_location_block(std::string & line, std::string & prevWord, bool *location_context, int *location_count);
// int open_location_block_2(std::string & line, std::string & prevWord, bool *location_context, int *location_count);
// int open_location_block_3(std::string & line, std::string & prevWord, bool *location_context, int *location_count);
// int close_location_block(std::string & line, std::string & prevWord, bool *location_context, int *location_count);

#endif
