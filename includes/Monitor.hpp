#ifndef MONITOR_HPP
# define MONITOR_HPP

#include <iostream>
#include <fstream>
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
#include "Request.hpp"

class Server;

# define LOG_PATH "logs/access.log"
# define CHUNK_SIZE 512
// # define KEEPALIVE 65

# define RED "\033[0;31m"
# define BLU "\033[0;34m"
# define WHI "\033[0m"

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
		void log(T message)
		{
			_accessStream << message;
			_accessStream.flush();
		}
		
		template<typename T, typename... Args>
		void log(T message, Args... args)
		{ 
			_accessStream << message;
			log(args...);
			_accessStream.flush();
		}

	private:
		// SERVERS
		std::deque<Server> _servers;

		// SOCKETS
		std::vector<int> _master_sockets;
		int _fd_count;
		int _fd_capacity;
		struct pollfd *_pfds;
		void _add_to_pfds(int new_fd);
		void _del_from_pfds(int i);
		void _print_events(struct pollfd *pfd) const;

		// LOG
		void _create_log_file(std::string const & filename, std::ofstream & stream);
		const std::string & _accessFile;
		std::ofstream _accessStream;
		void _exit_cerr_msg(std::string message, int code) const;
};

#endif
