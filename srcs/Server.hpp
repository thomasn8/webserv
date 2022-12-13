#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Config.hpp"
#include "Http.hpp"

#define LISTEN_BACKLOG 50
#define KEEPALIVE 65

class Server
{	
	public:
		Server(std::vector<Config> & configs);
		~Server();
		
	private:
		std::vector<Config> & _configs;
		int _keepalive_timeout;

		// int _server_fd;
		// int _client_fd;
		// size_t _client_read;
		// struct sockaddr_in _address;
		// char *_buffer;
		// std::string _response;

		void _createLogFile();
		const std::string	_accessFile;
		std::ofstream		_accessStream;

		void _exitWithError(std::ostream & stream, const std::string message, int code) const;
		std::ostream & _log(std::ostream & stream, const std::string message) const;

		// http classes ...

};

#endif
