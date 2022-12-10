#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <netinet/in.h>

#include "Log.hpp"

class Config
{	
	public:
		typedef unsigned long size_type;

		Config(const std::string file);
		Config();
		~Config();
		
		std::ofstream & getAccessStream() const;
		std::ofstream & getErrorStream() const;

		uint16_t getPort() const;
		uint32_t getAddress() const;
		size_type getClientMaxBodySize() const;

	private:
		const std::string	_configFile;
		std::ifstream		_configStream;
		
		void _createLogFile(const std::string accessFile, const std::string errorFile);
		const std::string	_accessFile;
		const std::string	_errorFile;
		std::ofstream		_accessStream;
		std::ofstream		_errorStream;

		uint16_t _port;
		uint32_t _address;				// si pas specifier: INADDR_ANY
		size_type _clientMaxBodySize;
};

#endif
