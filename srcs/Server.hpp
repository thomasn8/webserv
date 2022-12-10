#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
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
		typedef struct sockaddr_in t_sockaddr_in;
		typedef unsigned long size_type;

		Server();
		~Server();

		void parse_config_file(std::string file);
		void start_server();

	private:
		Config _config;

		int _server_fd;
		int _client_fd;
		size_type _client_read;
		t_sockaddr_in _address;
		char *_buffer;
		std::string _response;

		void start_error(std::string message, int code);


		// http classes ...

};

#endif
