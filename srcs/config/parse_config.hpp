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

// S_DIRECTIVE
# define S_D_LISTEN 0
# define S_D_SERVER_NAME 1
# define S_D_ROOT 2
# define S_D_INDEX 3
# define S_D_ERROR_PAGE 4
# define S_D_CLIENT_MAX_BODY_SIZE 5

// L_DIRECTIVE
# define L_D_ROOT 0
# define L_D_INDEX 1
# define L_D_METHODS 2
# define L_D_AUTOINDEX 3
# define L_D_REDIRECTION 4
# define L_D_UPLOADS_DIR 5
# define L_D_REDIRECT 6
# define L_D_CGI_BIN 7

typedef	int (*f_ptr_s)(std::string & line, bool *x_block, int *x_count);
typedef	int (*f_ptr_l)(std::string & line, std::string & prefix, int *x_block, int *x_count);

// parse_config
void parseConfig(std::string & configFile, Server & server);

// parse_context
int open_server_block(std::string & line, bool *server_context, int *server_count);
int open_server_block_2(std::string & line, bool *server_context, int *server_count);
int close_server_block(std::string & line, bool *server_context, int *server_count);
int open_location_block(std::string & line, std::string & prefix, int *location_context, int *location_count);
int open_location_block_2(std::string & line, std::string & prefix, int *location_context, int *location_count);
int close_location_block(std::string & line, std::string & prefix, int *location_context, int *location_count);

// utils
void exitWithError(std::ostream & stream, const std::string message, const std::string line, int code);
int tabLength(std::string *tab);
std::string TrimFunction(std::string str);
bool isNotBlank(std::string line);
bool isNotAlphaNum(std::string line);
bool prefixSyntax(std::string prefix);

#endif
