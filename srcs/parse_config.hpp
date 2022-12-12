#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Server.hpp"
#include "Config.hpp"
#include "split_quotes.hpp"

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs);

#endif
