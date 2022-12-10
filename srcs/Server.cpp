#include "Server.hpp"

Server::Server(Config const & config) :
_config(config)
{
}

Server::~Server()
{
}

void Server::startServer()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		exitWithError(_config.getErrorStream(), "socket failed\n", EXIT_FAILURE);
	
	_buffer = (char *)calloc(1, _config.getClientMaxBodySize());
	if (_buffer == NULL)
		exitWithError(_config.getErrorStream(), "buffer alloc error\n", EXIT_FAILURE);

	memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(_config.getAddress());
	_address.sin_port = htons(_config.getPort());
	_address.sin_len = sizeof(_address);
	
	if (bind(_server_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
		exitWithError(_config.getErrorStream(), "bind failed\n", EXIT_FAILURE);
	if (listen(_server_fd, LISTEN_BACKLOG) < 0)
		exitWithError(_config.getErrorStream(), "listen failed\n", EXIT_FAILURE);
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

void Server::stopServer()
{
	close(_server_fd);
	free(_buffer);
	// streams.inputFile.close();
}

void Server::exitWithError(std::ostream & stream, const std::string message, int code) const
{
	log(stream, message);
	exit(code);
}

std::ostream & Server::log(std::ostream & stream, const std::string message) const
{
	stream << message;
	return stream;
}

Server::Server() : _config(Config()) {}