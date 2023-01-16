#ifndef MONITOR_HPP
# define MONITOR_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <deque>

#include "Server.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "socket.hpp"

# define LOG_PATH "logs/access.log"
# define CHUNK_SIZE 1024
# define BUFFER_LIMIT 10000000 // 10MO

struct buffer_read {
	char *begin;
	char *current;
	size_t size;
	size_t capacity;
};

class Monitor
{
	public:
		typedef std::deque<Server>::iterator it_servers;

		// CONST/DESTR
		Monitor();
		~Monitor();

		// GETTERS/SETTERS
		std::deque<Server> & get_servers();
		void add_server();

		// SOCKETS
		void _prepare_master_sockets();
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
		int * _master_sockets;
		int _master_size;
		int _fd_count;
		int _fd_capacity;
		struct pollfd * _pfds;
		struct socket * _activeSockets; // les _master_size premiers pointeurs sont NULL, les suivants correspondent par index aux _pfds
		struct socket * _add_to_pfds(int new_fd, struct sockaddr_in * remoteAddr, Server * server);
		void _del_from_pfds(int i);
		void _accept_new_connection(int master_index);
		ssize_t _recv_all(int fd, struct socket & activeSocket, char *static_chunk);
		ssize_t _send_all(int i, const char * response, int size, struct socket & activeSocket);

		// RECV
		struct buffer_read _buf;

		// LOG
		void _create_log_file(std::string const & filename, std::ofstream & stream);
		const std::string & _accessFile;
		std::ofstream _accessStream;
		void _exit_cerr_msg(std::string message, int code);
};

#endif
