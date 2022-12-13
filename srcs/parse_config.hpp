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

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs);

typedef	int (*f_ptr)(std::string & line, std::string & prevWord, bool *server_block, int *server_count);
int check_open_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count);
int check_open_server_block_2(std::string & line, std::string & prevWord, bool *server_block, int *server_count);
int check_open_server_block_3(std::string & line, std::string & prevWord, bool *server_block, int *server_count);
int check_close_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count);

#endif
