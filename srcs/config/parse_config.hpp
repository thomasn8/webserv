#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../Server.hpp"
#include "Config.hpp"

# define INVALID 1
# define VALID 0
# define EQUAL 0
# define ERROR_MSG "Error: configuration file invalid:\n"
# define ERROR_SERVER_BLOCK "server{} directive error"
# define ERROR_LOCATION_BLOCK "location{} directive error"

typedef	int (*f_ptr_s)(std::string & line, bool *x_block, int *x_count);
typedef	int (*f_ptr_l)(std::string & line, std::string & prefix, int *x_block, int *x_count);

// parse_config
void parse_config(std::string & configFile, Server & server);

// parse_context
int open_server_block(std::string & line, bool *server_context, int *server_count);
int open_server_block_2(std::string & line, bool *server_context, int *server_count);
int close_server_block(std::string & line, bool *server_context, int *server_count);
int open_location_block(std::string & line, std::string & prefix, int *location_context, int *location_count);
int open_location_block_2(std::string & line, std::string & prefix, int *location_context, int *location_count);
int close_location_block(std::string & line, std::string & prefix, int *location_context, int *location_count);

// utils
void exit_with_error(std::ostream & stream, const std::string message, const std::string line, int code);
int tab_length(std::string *tab);
std::string trim_function(std::string str);
bool is_not_blank(std::string line);
bool is_not_alpha_num(std::string line);
bool prefix_syntax(std::string prefix);

#endif
