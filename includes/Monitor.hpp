#ifndef MONITOR_HPP
# define MONITOR_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <time.h>
// #include <chrono>
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
#include "responseInfos.hpp"
#include "utils.hpp"

# define LOG_PATH "logs/access.log"

// If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait for any file descriptor to become ready.
// If timeout is zero, then poll() will return without blocking. If the value of timeout is -1, the poll blocks indefinitely.
# define POLL_TIMEOUT -1

// read
# define CHUNK_RECV 1024
# define BUFFER_LIMIT 200000 // 200KO
# define RECV_TIMEOUT_MS 2000
struct buffer_read {
	char *begin;
	char *current;
	size_t size;
	size_t capacity;
};

// write
# define CHUNK_SEND 1024
# define SEND_TIMEOUT_MS 2000

class Monitor
{
	public:
		typedef std::deque<Server>::iterator it_servers;

		// CONST/DESTR
		Monitor(char **env);
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

	private:
		// SERVERS
		std::deque<Server> _servers;
		char **_env;

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
		int _replace_alone_header_cr(void);
		struct buffer_read _buf;
		void _check_buffer_capacity();
		ssize_t _recv_all(int fd, struct socket & activeSocket);
		int _send_all(int i, const char * response, int size, struct socket & activeSocket);
		uint64_t _recv_timeout[2];
		uint64_t _sent_timeout[2];

		// LOG
		// uint64_t _chrono_start;
		// uint64_t _chrono_stop;
		// void _start_chrono();
		// void _stop_chrono(int fd);
		void _create_log_file(std::string const & filename, std::ofstream & stream);
		std::string _logFile;
		std::ofstream _log;
		void _exit_cerr_msg(std::string message, int code);
};

#endif
