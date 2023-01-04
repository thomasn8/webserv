#ifndef MONITOR_HPP
# define MONITOR_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>
#include <deque>
#include <vector>

#include "Server.hpp"

# define LISTEN_BACKLOG 50
# define KEEPALIVE 65
# define LOG_PATH "logs/access.log"

class Monitor
{
	public:
		typedef std::deque<Server>::iterator it_servers;

		// CONST/DESTR
		Monitor();
		~Monitor();

		// GETTERS/SETTERS
		std::deque<Server> & get_servers();
		Server & get_last_server();
		void add_server();

		// SOCKETS
		void _prepare_sockets();
		void handle_connections();

		// LOG
		std::string get_time();
		void log_server_info();
	
		template <typename T>
		void log(T message) { _accessStream << message; }
		
		template<typename T, typename... Args>
		void log(T message, Args... args)
		{ 
			_accessStream << message;
			log(args...);
		}

	private:
		std::deque<Server> _servers;

		// SOCKETS
		std::vector<int> _master_sockets;
		int _fd_count;
		int _fd_capacity;
		struct pollfd *_pfds;
		void _add_to_pfds(int newfd);
		void _del_from_pfds(int i);


		// LOG
		void _create_log_file(std::string const & filename, std::ofstream & stream);
		const std::string & _accessFile;
		std::ofstream _accessStream;
		void _exit_cerr_msg(std::string message, int code) const;
};

#endif
