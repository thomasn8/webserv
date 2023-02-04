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
const std::string	g_server_directives[] = {"listen", "server_name", "root", "index", "error_page", "max_body_size", ""};
# define I_LISTEN_C 0
# define I_SERVER_NAME_C 1
# define I_ROOT_C 2
# define I_INDEX_C 3
# define I_ERROR_PAGE_C 4
# define I_MAX_BODY_SIZE_C 5

// SOCKET
# define BACKLOG 20
# define SEND_TIMEOUT_SEC 2
# define SEND_TIMEOUT_USEC 0

class Server
{
	public:
		typedef std::pair<int, std::string> error_page_pair;
		typedef std::deque<Location>::iterator loc_it;

		// CONST/DESTR
		Server();
		Server(const Server & src);
		~Server();

		// GETTERS/SETTERS
		// parse config
		std::deque<Location> & get_locations();
		Location & get_last_location();
		// handle_connection
		void add_location(std::string & route);
		void add_directive(int directiveIndex, std::string value);
		void set_address_port(std::string & value);
		void set_servername(std::string & value);
		void set_root(std::string & value);
		void set_index(std::string & value);
		void set_error_page(std::string & value);
		void set_max_body_size(std::string & value);
		uint16_t const & get_port() const;
		uint32_t const & get_ipv4() const;
		std::string const & get_port_str() const;
		std::string const & get_ipv4_str() const;
		std::string const & get_ipv4_port_str() const; 
		std::string const & get_servername() const;
		std::list<std::string> const & get_servernames() const;
		std::string const & get_root() const;
		std::list<std::string> const & get_indexes() const;
		std::list<error_page_pair> const & get_errorpages() const;
		size_t const & get_max_body_size() const;
		size_t const & get_maxrecv() const;
		struct sockaddr_in const & get_address() const;
		std::string _webserv_bin_path() const;

		// SOCKET
		int create_socket(char **env);

	private:
		char **_env;

		// CONFIG
		std::deque<Location> _locations;
		uint32_t _ipv4;
		std::string _ip;
		uint16_t _port;
		std::string _port_str;
		std::string _ipv4_str;
		std::string _ipv4_port_str;
		int _port_check(std::string & value);
		std::list<std::string> _serverNames;
		bool _defaultServerNames;
		std::string	_root;
		bool _defaultIndex;
		std::list<std::string> _indexFiles;
		size_t _maxBodySize;
		size_t _maxrecv;
		std::list<error_page_pair> _errorPages;

		// SOCKET
		int _socket_fd;
		struct sockaddr_in _address;

		// ERROR
		void _exit_cerr_msg(std::string message, int code) const;
};

#endif
