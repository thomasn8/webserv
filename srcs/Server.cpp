#include "../includes/Server.hpp"
/* 
	************ CONST/DESTR
*/
Server::Server() :
_ipv4(INADDR_ANY),
_ip(std::string("0.0.0.0")),
_port(htons(DEFAULT_PORT)),  
_serverNames(std::list<std::string>(1, std::string(DEFAULT_SERVERNAME))),
_defaultServerNames(true),
_root(std::string(_webserv_bin_path().append("/").append(DEFAULT_ROOT))),
_indexFiles(std::list<std::string>(1, std::string(DEFAULT_INDEX))),
_defaultIndex(true),
_maxBodySize(MBS),
_maxrecv(MHS + MBS),
_socket_fd(-1)
{}

Server::Server(const Server & src) :
_locations(src._locations),
_ipv4(src._ipv4),
_ip(src._ip),
_port(src._port),  
_serverNames(src._serverNames),
_defaultServerNames(src._defaultServerNames),
_root(src._root),
_indexFiles(src._indexFiles),
_defaultIndex(src._defaultIndex),
_maxBodySize(src._maxBodySize),
_maxrecv(src._maxrecv),
_errorPages(src._errorPages),
_socket_fd(src._socket_fd),
_address(src._address)
{}

Server::~Server() {}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Location> & Server::get_locations() { return _locations; }

Location & Server::get_last_location() { return _locations.back(); }

void Server::add_location(std::string & route) 
{
	if (route[0] == '.' )
		route.erase(0,1);
	else if (route[0] == '*')
		route.erase(0,2);
	for (loc_it it = _locations.begin(); it != _locations.end(); it++)
	{
		if ((*it).get_route() == route)
		{
			*it = Location(_root, _indexFiles);
			return;
		}
	}
	_locations.push_back(Location(_root, _indexFiles));
}

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

int Server::_port_check(std::string & value)
{
	int port = DEFAULT_PORT;
	try {
		size_t idx;
		port = std::stoi(value, &idx);
		if (value.substr(idx).size() != 0)
			_exit_cerr_msg("Error: invalid port", 1);
	}
	catch (const std::invalid_argument &ia) {
		_exit_cerr_msg("Error: invalid port", 1);
	}
	if (port != 80 && port < MIN_PORT_NO)
		_exit_cerr_msg("Error: invalid port: port number below 1024 (except default port 80) are not available\n", 1);
	if (port > MAX_PORT_NO)
		_exit_cerr_msg("Error: invalid port: maximum port number is 65535\n", 1);
	return htons(port);
}

// ip choisi par l'OS et la même pour chaque serveur, peu importe ce qui est spécifié dans la config
// pour utiliser une ip configurée, utiliser la variante commentée
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
			_ipv4 = INADDR_ANY;
			// if (before.compare("localhost") == 0)
			// 	_ipv4 = inet_addr("127.0.0.1");
			// else
			// 	_ipv4 = inet_addr(before.c_str());
		}
		if (!after.empty())
			_port = _port_check(after);
	}
	else
	{
		pos = value.find('.');
		if (pos > 0)
		{
			_ipv4 = INADDR_ANY;
			// if (value.compare("localhost") == 0)
			// 	_ipv4 = inet_addr("127.0.0.1");
			// else
			// 	_ipv4 = inet_addr(value.c_str());
		}
		else
			_port = _port_check(value);
	}
	_port_str = std::to_string(ntohs(_port));
	_ipv4_str = _ip;
	_ipv4_port_str = get_ipv4_str();
	_ipv4_port_str.append(":").append(get_port_str());
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
	// si relative, on complete la partie qui précède pour uniformiser les path en absolute
	if (value[0] != '/')
		_root = _webserv_bin_path().append("/").append(value);
	else
		_root = value;
	
	// toujours checker qu'il n'y ait pas de slash à la fin, question d'uniformité
	// quand on joint 2 paths, on sait qu'on doit toujours append("/") entre 2
	if (_root[_root.size() - 1] == '/')
		_root.erase(_root.size() - 1);
}

void Server::set_index(std::string & value)
{
	if (value.empty())
		return;
	if (value[0] == '/')
		value.erase(0, 1);
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
		size_t idx;
		statusCode = std::stoi(value, &idx);
		if (value.substr(idx).size() != 0)
			throw std::invalid_argument("asdf");
	}
	catch (const std::invalid_argument &ia) {
		// itere sur tous les statusCode-errorFile pair depuis la fin
		// et ajoute le errorFile aux pairs qui ont que le statusCode
		std::list< error_page_pair >::reverse_iterator rit = _errorPages.rbegin();
		std::list< error_page_pair >::reverse_iterator rite = _errorPages.rend();
		for (; rit!= rite; ++rit)
		{
			if ((*rit).second.empty())
			{
				// si relative, on complete la partie qui précède pour uniformiser les path en absolute
				if (value[0] != '/')
					(*rit).second = std::string(_root).append("/").append(value);
				else
					(*rit).second = value;
			}
		}
		return;
	}
	_errorPages.push_back(std::pair(statusCode, ""));
}

void Server::set_client_max_body_size(std::string & value)
{
	if (value.empty())
		return;
	size_t mbs = MBS;
	try {
		size_t idx;
		mbs = std::stoi(value, &idx);
		std::string unit = value.substr(idx);
		for (int i = 0; i < unit.size(); i++)
			unit[i] = toupper(unit[i]);
		if (unit.size() != 0)
		{
			if (unit.compare("B") == 0)
				mbs *= 1;
			else if (unit.compare("KB") == 0 || unit.compare("KO") == 0 || unit.compare("K") == 0)
				mbs *= 1000;
			else if (unit.compare("MB") == 0 || unit.compare("MO") == 0 || unit.compare("M") == 0)
				mbs *= 1000 * 1000;
			else if (unit.compare("GB") == 0 || unit.compare("GO") == 0 || unit.compare("G") == 0)
				mbs *= 1000 * 1000 * 1000;
			else
				_exit_cerr_msg("Error: invalid max_body_size format. Examples: 4000, 300KB, 2M\n", 1);
		}
	}
	catch (const std::invalid_argument &ia) {
		_exit_cerr_msg("Error: invalid max_body_size format. Examples: 4000, 300KB, 2M\n", 1);
	}
	if (mbs > MAX_MBS)
		_exit_cerr_msg("Error: max_body_size too large: maximum of 1GB\n", 1);
	_maxBodySize = mbs;
	_maxBodySize ? _maxrecv = _maxBodySize + MHS : _maxrecv = 0;
}

uint16_t const &Server::get_port() const { return _port; }

uint32_t const &Server::get_ipv4() const { return _ipv4; }

std::string const &Server::get_port_str() const { return _port_str; }

std::string const &Server::get_ipv4_str() const { return _ipv4_str; }

std::string const &Server::get_ipv4_port_str() const { return _ipv4_port_str; }

std::string const &Server::get_servername() const { return _serverNames.front(); }

std::list<std::string> const &Server::get_servernames() const { return _serverNames; }

std::string const &Server::get_root() const { return _root; }

std::list<std::string> const &Server::get_indexes() const { return _indexFiles; }

// max body size
size_t const &Server::get_max_body_size() const { return _maxBodySize; }

// max body size + max header size
size_t const &Server::get_maxrecv() const { return _maxrecv; }

std::list<Server::error_page_pair> const & Server::get_errorpages() const { return _errorPages; }

struct sockaddr_in const & Server::get_address() const { return _address; }

std::string Server::_webserv_bin_path() const
{
	char * bin = getcwd(NULL, 0);
	std::string bin_str = bin;
	free(bin);
	return bin_str;
}

/* 
	************ SOCKET
*/
int Server::create_socket(char **env)
{
	_env = env;
	int opt = 1;
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
		_exit_cerr_msg("Error: impossible to run server(s): socket() failed\n", 1);
	fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): setsockopt() no 1 failed\n", 1);
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&opt, sizeof(opt)) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): setsockopt() no 2 failed\n", 1);
	
	struct timeval timeout;      
    timeout.tv_sec = SEND_TIMEOUT_SEC;
    timeout.tv_usec = SEND_TIMEOUT_USEC;
    if (setsockopt (_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
        _exit_cerr_msg("Error: impossible to run server(s): setsockopt() no 3 failed\n", 1);
    if (setsockopt (_socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
        _exit_cerr_msg("Error: impossible to run server(s): setsockopt() no 4 failed\n", 1);

	memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = _ipv4;
	_address.sin_port = _port;
	_ip = std::string(inet_ntoa(_address.sin_addr));

	if (bind(_socket_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): bind() failed\n", 1);
	if (listen(_socket_fd, BACKLOG) < 0)
		_exit_cerr_msg("Error: impossible to run server(s): listen() failed\n", 1);
	return _socket_fd;
}

/* 
	************ ERROR
*/
void Server::_exit_cerr_msg(const std::string message, int code) const
{
	std::cerr << message;
	exit(code);
}
