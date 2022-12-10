#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <netinet/in.h>

#define DEFAULT_PORT 8080

class Config
{	
	public:
		Config(const std::string file);
		~Config();

		std::ofstream & getAccessStream();
		std::ofstream & getErrorStream();

		uint16_t getPort() const;
		uint32_t getAddress() const;
		size_t getClientMaxBodySize() const;

	private:
		const std::string	_configFile;
		std::ifstream		_configStream;
		
		void _createLogFile(const std::string accessFile, const std::string errorFile);
		const std::string	_accessFile;
		const std::string	_errorFile;
		std::ofstream		_accessStream;
		std::ofstream		_errorStream;

		const uint16_t _port;
		const uint32_t _address;
		const size_t _clientMaxBodySize;
};

#endif
