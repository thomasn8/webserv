#include "../includes/Server.hpp"
/* 
	************ CONST/DESTR
*/
Server::Server() :
_locations(std::deque<Location>()),
_ipv4(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_defaultServerNames(true),
_root(std::string(DEFAULT_ROOT)),
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))),
_defaultIndex(true),
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >()),
_socket_fd(-1),
_address()
{}

Server::Server(const Server & src) :
_locations(std::deque<Location>()),
_ipv4(INADDR_ANY), _port(DEFAULT_PORT),  
_serverNames(std::vector<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_defaultServerNames(true),
_root(std::string(DEFAULT_ROOT)),
_indexFiles(std::vector<std::string>(1, std::string(DEFAULT_INDEX))),
_defaultIndex(true),
_clientMaxBodySize(MBS),
_errorPages(std::vector< error_page_pair >()),
_socket_fd(-1),
_address()
{ (void) src; }

Server::~Server() {}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Location> & Server::get_locations() { return _locations; }

Location & Server::get_last_location() { return get_locations().back(); }

void Server::add_location() {_locations.push_back(Location()); }

void Server::add_directive(int directiveIndex, std::string value)
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

void Server::set_address_port(std::string & value)
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
				_ipv4 = inet_addr("127.0.0.1");
			_ipv4 = inet_addr(before.c_str());
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
				_ipv4 = inet_addr("127.0.0.1");
			_ipv4 = inet_addr(value.c_str());
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

void Server::set_servername(std::string & value)
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

void Server::set_root(std::string & value)
{
	if (value.empty())
		return;
	_root = value;
}

void Server::set_index(std::string & value)
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

void Server::set_error_page(std::string & value)
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

void Server::set_client_max_body_size(std::string & value)
{
	if (value.empty())
		return;
	_clientMaxBodySize = std::stoi(value);
}

uint16_t Server::get_port() const { return _port; }

uint32_t Server::get_ipv4() const { return _ipv4; }

std::string Server::get_ip() const 
{
	char ip4[INET_ADDRSTRLEN];
	return std::string(inet_ntop(AF_INET, &(_address.sin_addr), ip4, INET_ADDRSTRLEN));
}

std::string Server::get_servername() const { return _serverNames.front(); }

std::vector<std::string> & Server::get_servernames() { return _serverNames; }

std::string Server::get_root() const { return _root; }

std::string Server::get_index() const { return _indexFiles.front(); }

std::vector<std::string> & Server::get_indexes() { return _indexFiles; }

size_t Server::get_client_max_body_size() const { return _clientMaxBodySize; }

std::vector<Server::error_page_pair> & Server::get_errorpages() { return _errorPages; }

struct sockaddr_in & Server::get_address() { return _address; }

/* 
	************ SOCKET
*/
void Server::_exit_cerr_msg(const std::string message, int code) const
{
	std::cerr << message;
	exit(code);
}

int Server::create_socket()
{
	int opt = 1;
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
		_exit_cerr_msg("Error: impossible to run server(s): socket() failed", 1);
	fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): setsockopt() failed", 1);
	
	memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);	// a voir si besoin d'utiliser _ipv4 (pour l'instant l'OS choisi l'ip)
	_address.sin_port = _port;
	_address.sin_len = sizeof(_address);

	if (bind(_socket_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): bind() failed", 1);
	if (listen(_socket_fd, BACKLOG) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): listen() failed", 1);
	return _socket_fd;
}
