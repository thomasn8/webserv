#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config(const std::string file) :
_configFile(file), _configStream(),
_accessFile(), _errorFile(),
_accessStream(), _errorStream(),
_port(0), _address(INADDR_ANY), _clientMaxBodySize(0)
{
	_configStream.open(_configFile, std::ifstream::in);
	if (_configStream.fail() == true)
	{
		std::cerr << "Error while opening configuration file." << std::endl;
		exit(1);
	}
	// std::string accessFile = "../conf/logs/access.log";
	// std::string errorFile = "../conf/logs/error.log";
	std::string accessFile;
	std::string errorFile;
	// parsing de _configStream
	// ...

	_createLogFile(accessFile, errorFile);
}

Config::~Config()
{
	_accessStream.close();
	_errorStream.close();
}

/* 
	************ DISABLED (private constructor)
*/
Config::Config() :
_configFile(), _configStream(),
_accessFile(), _errorFile(),
_accessStream(), _errorStream(),
_port(0), _address(INADDR_ANY), _clientMaxBodySize(0)
{
}

/* 
	************ GETTERS
*/
std::ofstream & Config::getAccessStream() const
{
	return _accessStream;
}

std::ofstream & Config::getErrorStream() const
{
	return _errorStream;
}

uint16_t Config::getPort() const
{
	return _port;
}

uint32_t Config::getAddress() const
{
	return _address;
}

size_type Config::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

/* 
	************ OTHER
*/
void Config::_createLogFile(const std::string accessFile, const std::string errorFile)
{
	std::string	file;
	if (accessFile.empty())
		file = "../conf/logs/access.log";
	else
		file = accessFile;
	_accessStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_accessStream.fail() == true)
	{
		std::cerr << "Error while creating access log's file: " << file << std::endl;
		exit(1);
	}
	file.clear();
	if (errorFile.empty())
		file = "../conf/logs/error.log";
	else
		file = errorFile;
	_errorStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_errorStream.fail() == true)
	{
		std::cerr << "Error while creating error log's file: " << file << std::endl;
		exit(1);
	}
}
