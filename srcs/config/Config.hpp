#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <deque>

#include "Location.hpp"

// INITIALIZATION
# define DEFAULT_IP "0.0.0.0" // default IP address that lets the operating system choose
# define DEFAULT_PORT 80
# define DEFAULT_SERVERNAME "localhost"
# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define MBS 0 // Client Max Body Size
# define DEFAULT_CGI_PATH "cgi/"
# define DEFAULT_CGI_EXT "php"

// DIRECTIVE INDEX
const std::string	g_server_directives[] = {"listen", "server_name", "root", "index", "error_page", "client_max_body_size", ""};
# define I_LISTEN_C 0
# define I_SERVER_NAME_C 1
# define I_ROOT_C 2
# define I_INDEX_C 3
# define I_ERROR_PAGE_C 4
# define I_CLIENT_MAX_BODY_SIZE_C 5

class Config
{
	public:
		typedef std::pair<int, std::string> error_page_pair;

		// CONST/DESTR
		Config();
		Config(const Config & src);
		~Config();

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
		uint32_t get_address() const;
		std::string get_servername() const;
		std::vector<std::string> & get_servernames();
		std::string get_root() const;
		std::string get_index() const;
		std::vector<std::string> & get_indexes();
		std::vector<error_page_pair> & get_errorpages();
		size_t get_client_max_body_size() const;

	private:
		std::deque<Location> _locations;

		uint32_t _address;
		uint16_t _port;
		std::vector<std::string> _serverNames;
		bool _defaultServerNames;

		std::string	_root;
		std::vector<std::string> _indexFiles;
		bool _defaultIndex;
		size_t _clientMaxBodySize;
		
		std::vector< error_page_pair > _errorPages;
};

#endif
