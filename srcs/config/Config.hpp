#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

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
# define C_LISTEN 0
# define C_SERVER_NAME 1
# define C_ROOT 2
# define C_INDEX 3
# define C_ERROR_PAGE 4
# define C_CLIENT_MAX_BODY_SIZE 5

class Config
{
	public:
		typedef std::pair<int, std::string> error_page_pair;

		// CONST/DESTR
		Config();
		Config(const Config & src);
		~Config();

		// GETTERS/SETTERS
		// parse config
		std::vector<Location> & get_locations();
		Location & get_last_location();
		void add_location();
		void add_directive(int directiveIndex, std::string value);
		void set_address_port(std::string & value);
		void set_servername(std::string & value);
		void set_root(std::string & value);
		void set_index(std::string & value);
		void set_error_page(std::string & value);
		void set_client_max_body_size(std::string & value);
		// sockets
		uint16_t get_port() const;
		uint32_t get_address() const;
		size_t get_client_max_body_size() const;


	private:
		std::vector<Location> _locations;

		uint32_t _address;
		uint16_t _port;
		std::vector<std::string> _serverNames;

		std::string	_root;
		std::vector<std::string> _indexFiles;
		size_t _clientMaxBodySize;
		
		std::vector< error_page_pair > _errorPages;
};

#endif
