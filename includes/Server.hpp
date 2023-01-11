#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <list>
#include <deque>
#include <stdexcept>

#include "Location.hpp"

// INITIALIZATION
# define DEFAULT_PORT 80
# define MIN_PORT_NO 1024
# define MAX_PORT_NO 65535
# define DEFAULT_SERVERNAME "localhost"
# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define MHS 8000
# define MBS 1000000 // 1M
# define MAX_MBS 1000000000 // 100M

// DIRECTIVE INDEX
// (pour en rajouter/modifier: modifier le tableau + definir une macro pour l'index + modifier le switch-case dans add_directive() et creer les getter/setter)
const std::string	g_server_directives[] = {"listen", "server_name", "root", "index", "error_page", "client_max_body_size", ""};
# define I_LISTEN_C 0
# define I_SERVER_NAME_C 1
# define I_ROOT_C 2
# define I_INDEX_C 3
# define I_ERROR_PAGE_C 4
# define I_CLIENT_MAX_BODY_SIZE_C 5

// SOCKET
# define BACKLOG 10

class Server
{
	public:
		typedef std::pair<int, std::string> error_page_pair;

		// CONST/DESTR
		Server();
		Server(const Server & src);
		~Server();

		// GETTERS/SETTERS
		std::deque<Location> & get_locations();
		Location & get_last_location();
		void add_location();
		void add_directive(int directiveIndex, std::string value);
		void set_address_port(std::string & value);
		void set_servername(std::string & value);
		void set_root(std::string & value);
		void set_index(std::string & value);
		void set_error_page(std::string & value);
		void set_client_max_body_size(std::string & value);
		uint16_t get_port() const;
		uint32_t get_ipv4() const;
		std::string get_port_str() const;
		std::string get_ipv4_str() const;
		std::string get_ipv4_port_str() const; 
		std::string get_servername() const;
		std::list<std::string> & get_servernames();
		std::string get_root() const;
		std::list<std::string> & get_indexes();
		std::list<error_page_pair> & get_errorpages();
		size_t get_client_max_body_size() const;
		size_t get_maxrecv() const;
		struct sockaddr_in & get_address();
		std::string _webserv_bin_path() const;

		// SOCKET
		int create_socket();

	private:
		// CONFIG
		std::deque<Location> _locations;
		uint32_t _ipv4;
		uint16_t _port;
		int _port_check(std::string & value);
		std::list<std::string> _serverNames;
		bool _defaultServerNames;
		std::string	_root;
		bool _defaultIndex;
		std::list<std::string> _indexFiles;
		size_t _clientMaxBodySize;
		size_t _maxrecv;
		std::list<error_page_pair> _errorPages;

		// SOCKET
		int _socket_fd;
		struct sockaddr_in _address;

		// ERROR
		void _exit_cerr_msg(std::string message, int code) const;
};

#endif
