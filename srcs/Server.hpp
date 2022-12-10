#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
// #include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Config.hpp"
#include "Http.hpp"

#define LISTEN_BACKLOG 50

class Server
{	
	public:
		Server(Config & config);
		~Server();
		
	private:
		Config & _config;

		int _server_fd;
		int _client_fd;
		size_t _client_read;
		struct sockaddr_in _address;
		char *_buffer;
		std::string _response;

		void exitWithError(std::ostream & stream, const std::string message, int code) const;
		std::ostream & log(std::ostream & stream, const std::string message) const;

		// http classes ...

};

#endif
