#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config() :
_locations(std::deque<Location>()),
_address(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_defaultServerNames(true),
_root(std::string(DEFAULT_ROOT)),
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))),
_defaultIndex(true),
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >())
{}

Config::Config(const Config & src) :
_locations(std::deque<Location>()),
_address(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_defaultServerNames(true),
_root(std::string(DEFAULT_ROOT)),
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))),
_defaultIndex(true),
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >())
{ (void) src; }

Config::~Config() {}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Location> & Config::get_locations() { return _locations; }

Location & Config::get_last_location() { return get_locations().back(); }

void Config::add_location() {_locations.push_back(Location()); }

void Config::add_directive(int directiveIndex, std::string value)
{
	switch (directiveIndex) 
	{
		case I_LISTEN_C:
			set_address_port(value);
			break;
		case I_SERVER_NAME_C:
			set_servername(value);
			break;
		case I_ROOT_C:
			set_root(value);
			break;
		case I_INDEX_C:
			set_index(value);
			break;
		case I_ERROR_PAGE_C:
			set_error_page(value);
			break;
		case I_CLIENT_MAX_BODY_SIZE_C:
			set_client_max_body_size(value);
			break;
	}
}

void Config::set_address_port(std::string & value)
{
	if (value.empty())
		return;
	int pos = value.find(':');
	if (pos > 0)
	{
		std::string before = value.substr(0, pos);
		std::string after  = value.substr(pos+1);
		if (!before.empty())
		{
			if (before.compare("localhost") == 0)
				_address = inet_addr("127.0.0.1");
			_address = inet_addr(before.c_str());
		}
		if (!after.empty())
		{
			int port = DEFAULT_PORT;
			try {
				port = std::stoi(after);
			}
			catch (const std::invalid_argument &ia) {
			}
			_port = htons(port);
		}
	}
	else
	{
		pos = value.find('.');
		if (pos > 0)
		{
			if (value.compare("localhost") == 0)
				_address = inet_addr("127.0.0.1");
			_address = inet_addr(value.c_str());
		}
		else
		{
			int port = DEFAULT_PORT;
			try {
				port = std::stoi(value);
			}
			catch (const std::invalid_argument &ia) {
			}
			_port = htons(port);
		}
	}
}

void Config::set_servername(std::string & value)
{
	if (value.empty())
		return;
	if (_defaultServerNames == true)
	{
		_serverNames.pop_back();
		_serverNames.push_back(value);
		_defaultServerNames = false;
	}
	else
		_serverNames.push_back(value);
}

void Config::set_root(std::string & value)
{
	if (value.empty())
		return;
	_root = value;
}

void Config::set_index(std::string & value)
{
	if (value.empty())
		return;
	if (_defaultIndex == true)
	{
		_indexFiles.pop_back();
		_indexFiles.push_back(value);
		_defaultIndex = false;
	}
	else
		_indexFiles.push_back(value);
}

void Config::set_error_page(std::string & value)
{
	if (value.empty())
		return;

	int statusCode = 0;	
	try {
		statusCode = std::stoi(value);
	}
	catch (const std::invalid_argument &ia) {
		// itere sur tous les statusCode-errorFile pair depuis la fin
		// et ajoute le errorFile aux pairs qui ont que le statusCode
		std::vector< error_page_pair >::reverse_iterator rit = _errorPages.rbegin();
		std::vector< error_page_pair >::reverse_iterator rite = _errorPages.rend();
		for (; rit!= rite; ++rit)
		{
			if ((*rit).second.empty())
				(*rit).second = value;
		}
		return;
	}
	_errorPages.push_back(std::pair(statusCode, ""));
}

void Config::set_client_max_body_size(std::string & value)
{
	if (value.empty())
		return;
	_clientMaxBodySize = std::stoi(value);
}

uint16_t Config::get_port() const { return _port; }

uint32_t Config::get_address() const { return _address; }

std::string Config::get_servername() const { return _serverNames.front(); }

std::vector<std::string> & Config::get_servernames() { return _serverNames; }

std::string Config::get_root() const { return _root; }

std::string Config::get_index() const { return _indexFiles.front(); }

std::vector<std::string> & Config::get_indexes() { return _indexFiles; }

size_t Config::get_client_max_body_size() const { return _clientMaxBodySize; }

std::vector<Config::error_page_pair> & Config::get_errorpages() { return _errorPages; }
