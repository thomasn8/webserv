#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <list>
#include <vector>

#include "config/Config.hpp"

# define LISTEN_BACKLOG 50
# define KEEPALIVE 65
# define LOG_PATH "logs/access.log"

class Server
{	
	public:
		// CONST/DESTR
		Server();
		~Server();

		// GETTERS/SETTERS
		std::list<Config> & get_configs();
		Config & get_last_config();
		void add_config();

		// SOCKETS

		// LOG
		std::string get_time();
		void cout_config_info();
	
		template <typename T>
		void log(T message) { _accessStream << message; }
		
		template<typename T, typename... Args>
		void log(T message, Args... args)
		{ 
			_accessStream << message;
			log(args...);
		}

	private:
		std::list<Config> _configs;

		// SOCKETS
		// int _server_fd;
		// int _client_fd;
		// size_t _client_read;
		// struct sockaddr_in _address;
		// char *_buffer;
		// std::string _response;

		// LOG
		void _create_log_file(std::string const & filename, std::ofstream & stream);
		const std::string & _accessFile;
		std::ofstream _accessStream;
		void _exit_cerr_msg(std::string message, int code) const;
};

#endif
