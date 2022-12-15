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
		// parse config
		std::vector<Config> & get_configs();
		Config & get_last_config();
		void add_config();
		// sockets

		// LOG
		std::string get_time();
		template <typename T>
		void log(T message) { _accessStream << message; }
		template<typename T, typename... Args>
		void log(T message, Args... args) // recursive variadic function
		{ _accessStream << message; log(args...); }

	private:
		std::vector<Config> _configs;

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
		void _exit_with_error(std::ostream & stream, std::string message, int code) const;
		void _log_exit(const std::string message, int code);
};

#endif
