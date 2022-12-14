#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <vector>

#include "Location.hpp"

# define DEFAULT_IP "0.0.0.0" // default IP address that lets the operating system choose
# define DEFAULT_PORT 80
# define DEFAULT_SERVERNAME "localhost"
# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define MBS 0 // Client Max Body Size
# define DEFAULT_CGI_PATH "cgi/"
# define DEFAULT_CGI_EXT "php"


class Config
{
	// friend class Server;

	public:
		typedef std::pair<int, std::string> error_page_pair;

		Config();
		Config(const Config & src);
		~Config();

		uint16_t getPort() const;
		uint32_t getAddress() const;
		size_t getClientMaxBodySize() const;

		std::vector<Location> & getLocations();
		void addLocation();

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
