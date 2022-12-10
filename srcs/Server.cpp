#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::parse_config_file(std::string file)
{

}

void Server::start_error(std::string message, int code)
{
	perror(message.c_str()); 
	exit(code); 
}

void Server::start_server()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		start_error("socket failed", EXIT_FAILURE);
	memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(_config.getAddress());
	_address.sin_port = htons(_config.getPort());
	_address.sin_len = sizeof(_address);
	_buffer = (char *)calloc(1, _config.getClientMaxBodySize());
	if (_buffer == NULL)
		start_error("buffer alloc error", EXIT_FAILURE);
	if (bind(_server_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
		start_error("bind failed", EXIT_FAILURE);
	if (listen(_server_fd, LISTEN_BACKLOG) < 0)
		start_error("listen failed", EXIT_FAILURE);
    // _response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!";
}
