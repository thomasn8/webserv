#include "Server.hpp"

/* 
	************ CONST/DESTR
*/
Server::Server() :
_configs(std::vector<Config>()),
// _server_fd(), _client_fd(), _client_read(), 
// _address(), _buffer(), _response(),
_accessFile(std::string(LOG_PATH)), _accessStream()
{
	_createLogFile(_accessFile, _accessStream);
	
	
	// lancement du server
	// _server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// if (_server_fd < 0)
	// 	_exitWithError(_config.getErrorStream(), "socket failed\n", EXIT_FAILURE);
	
	// _buffer = (char *)calloc(1, _config.getClientMaxBodySize());
	// if (_buffer == NULL)
	// 	_exitWithError(_config.getErrorStream(), "buffer alloc error\n", EXIT_FAILURE);

	// memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	// _address.sin_family = AF_INET;
	// _address.sin_addr.s_addr = htonl(_config.getAddress());
	// _address.sin_port = htons(_config.getPort());
	// _address.sin_len = sizeof(_address);
	
	// if (bind(_server_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
	// 	_exitWithError(_config.getErrorStream(), "bind failed\n", EXIT_FAILURE);
	// if (listen(_server_fd, LISTEN_BACKLOG) < 0)
	// 	_exitWithError(_config.getErrorStream(), "listen failed\n", EXIT_FAILURE);
    // _response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!";


	// while(1)
	// {
	// 	std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
	// 	client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	// 	if (client_fd < 0)
	// 	{
	// 		perror("accept failed");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	client_read = read(client_fd, buffer, 30000);
	// 	if (client_read < 0)
	// 		perror("No bytes are there to read");
	// 	std::cout << buffer << std::endl;
	// 	write(client_fd, response.c_str(), response.length());
	// 	std::cout << "------- Hello message sent -------" << std::endl;
	// 	close(client_fd);
	// }
}

Server::~Server()
{
	// close(_server_fd);
	// free(_buffer);
}

/* 
	************ GETTERS/SETTERS
*/
// parsing config
std::vector<Config> & Server::getConfigs() { return _configs; }
Config & Server::getLastConfig() { return getConfigs().back(); }
void Server::addConfig() { _configs.push_back(Config()); }

// sockets
// ...

/* 
	************ SOCKETS
*/
// ...

/* 
	************ LOG
*/
void Server::_createLogFile(std::string const & filename, std::ofstream & stream)
{
	stream.open(filename, std::ofstream::out | std::ofstream::app);
	if (stream.fail() == true)
		_exitWithError(std::cerr, "Error while creating access log file\n", 1);
}

std::string Server::getTime()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80] = {0};
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void Server::_exitWithError(std::ostream & stream, const std::string message, int code) const
{
	std::cerr << message;
	exit(code);
}

void Server::_logExit(std::string message, int code)
{
	log(message);
	exit(code);
}
