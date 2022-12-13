#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <vector>
#include <array>

#define DEFAULT_IP "0.0.0.0" // default IP address that lets the operating system choose
#define DEFAULT_PORT 80
#define MBS 0				// Client Max Body Size

class Config
{	
	public:
		Config();
		~Config();

		std::ofstream & getAccessStream();
		std::ofstream & getErrorStream();

		uint16_t getPort() const;
		uint32_t getAddress() const;
		size_t getClientMaxBodySize() const;

	private:
		// address
		const uint32_t _address;
		const uint16_t _port;
		std::vector<std::string> & _serverNames;

		// other
		const std::string _root;
		std::vector<std::string> & _indexFiles;
		const size_t _clientMaxBodySize;
		const std::string _cgiBinPath;
		const std::string _cgiExtension;
		
		// error pages
		std::vector<std::pair< int, std::string> > & _errorPages;

		// routes
		std::vector<Location> & _Location;

};

class Location
{	
	public:
		Location();
		~Location();

	private:
		const std::string _prefix;	//   /images/
		const int _prefixCount;

		const std::string _root;
		std::vector<std::string> & _indexFiles;
		
		const int _methods;	// Get=4 + Post=2 + Delete=1
		const std::string _uploadsDir;
		
		const bool _autoindex;
		const std::string _dirList;

		// array[old_url, new_url, statusCode]
		std::vector< std::array<std::string, 3> > & _redirections;

		const std::string _cgiBinPath;
		const std::string _cgiExtension;
};

#endif
