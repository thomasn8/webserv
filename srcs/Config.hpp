#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <vector>

#define DEFAULT_IP "0.0.0.0" // default IP address that lets the operating system choose
#define DEFAULT_PORT 80
#define MBS 0

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
		void _createLogFile(const std::string accessFile, const std::string errorFile);
		const std::string	_accessFile;
		const std::string	_errorFile;
		std::ofstream		_accessStream;
		std::ofstream		_errorStream;

		std::vector<std::string> _serverNames;
		const uint32_t _address;
		const uint16_t _port;
		const size_t _clientMaxBodySize;

		void _exitWithError(std::ostream & stream, const std::string message, int code) const;
		std::ostream & _log(std::ostream & stream, const std::string message) const;

};

#endif
