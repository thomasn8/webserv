#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config() :
_locations(std::vector<Location>()),
_address(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_root(std::string(DEFAULT_ROOT)), 
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))), 
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >()) 
{}

Config::Config(const Config & src) :
_locations(std::vector<Location>()),
_address(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_root(std::string(DEFAULT_ROOT)), 
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))), 
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >())
{ (void) src;}

Config::~Config() {}

/* 
	************ GETTERS/SETTERS
*/
// parse config
std::vector<Location> & Config::getLocations() { return _locations; }
Location & Config::getLastLocation() { return getLocations().back(); }
void Config::addLocation() {_locations.push_back(Location()); }
void Config::addDirective(int directiveIndex, std::string value)
{
	// switch case (utiliser les MACROS)
		// avec setter pour chaque directive
}

// sockets
uint16_t Config::getPort() const { return _port; }
uint32_t Config::getAddress() const { return _address; }
size_t Config::getClientMaxBodySize() const { return _clientMaxBodySize; }
// ...
