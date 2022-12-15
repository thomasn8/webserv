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
std::vector<Location> & Config::get_locations() { return _locations; }
Location & Config::get_last_location() { return get_locations().back(); }
void Config::add_location() {_locations.push_back(Location()); }
void Config::add_directive(int directiveIndex, std::string value)
{
	// switch case (utiliser les MACROS)
		// avec setter pour chaque directive
}

// sockets
uint16_t Config::get_port() const { return _port; }
uint32_t Config::get_address() const { return _address; }
size_t Config::get_client_max_body_size() const { return _clientMaxBodySize; }
// ...
