#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../../includes/Monitor.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Location.hpp"

# define ROUTE_SYNTAX_INVALID 2
# define INVALID 1
# define VALID 0
# define EQUAL 0
# define BRACKET_CLOSED 0
# define ERROR_MSG "Error: configuration file invalid:\n"
# define ERROR_SERVER_BLOCK "server block brackets unclosed"
# define ERROR_LOCATION_BLOCK "location block brackets unclosed"
# define ERROR_LOCATION_SYNTAX "Error: location blocks must begin with /[...] for routes or with .[...] for cgi:\n"

typedef	int (*f_ptr_s)(std::string & line, bool *x_block, int *x_count);
typedef	int (*f_ptr_l)(std::string & line, std::string & route, int *x_block, int *x_count);

// parse_config
void parse_config(std::string & configFile, Monitor & monitor);

// parse_context
int open_server_block(std::string & line, bool *server_context, int *server_count);
int open_server_block_2(std::string & line, bool *server_context, int *server_count);
int close_server_block(std::string & line, bool *server_context, int *server_count);
int open_location_block(std::string & line, std::string & route, int *location_context, int *location_count);
int open_location_block_2(std::string & line, std::string & route, int *location_context, int *location_count);
int close_location_block(std::string & line, std::string & route, int *location_context, int *location_count);

// utils
void p_exit_cerr_msg(const std::string message, const std::string line, int code);
int p_tablen(const std::string *tab);
std::string p_trim_sides(std::string str);
bool p_isnotblank(std::string line);
bool p_isnotalphanum(std::string line);
bool p_route_syntax(std::string route);
bool p_error_page_syntax(std::string line);
bool p_method_syntax(std::string word);
bool p_redirect_syntax(std::string line);

#endif
