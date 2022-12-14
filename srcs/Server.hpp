#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

#include "config/Config.hpp"

# define LISTEN_BACKLOG 50
# define KEEPALIVE 65
# define LOG_PATH "logs/access.log"

class Server
{	
	public:
		Server();
		~Server();

		std::vector<Config> const & getConfigs() const;
		void addConfig();
		
	private:
		std::vector<Config> _configs;

		// int _server_fd;
		// int _client_fd;
		// size_t _client_read;
		// struct sockaddr_in _address;
		// char *_buffer;
		// std::string _response;

		void _createLogFile(std::string const & filename, std::ofstream & stream);
		const std::string & _accessFile;
		std::ofstream _accessStream;

		void _exitWithError(std::ostream & stream, const std::string message, int code) const;
		std::ostream & _log(std::ostream & stream, const std::string message) const;
};

#endif
