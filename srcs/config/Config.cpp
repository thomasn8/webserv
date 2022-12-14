#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config() : 
_address(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_root(std::string(DEFAULT_ROOT)), 
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))), 
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >()),
_locations(std::vector<Location>())
{
	// si specifier dans config, creer des log files specifiques dedie pour le server en question
	// _createLogFile(accessFile, errorFile);
}

Config::~Config() {}

/* 
	************ GETTERS
*/
uint16_t Config::getPort() const
{
	return _port;
}

uint32_t Config::getAddress() const
{
	return _address;
}

size_t Config::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

/* 
	************ OTHER
*/
