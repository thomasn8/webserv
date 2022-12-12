#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>
#include "Server.hpp"
#include "Config.hpp"

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs);

#endif
