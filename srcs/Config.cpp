#include "Config.hpp"

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
	// parsing de _configStream
	// ...
	std::string accessFile;
	std::string errorFile;

	_createLogFile(accessFile, errorFile);
}

Config::Config() :
_configFile(), _configStream(),
_accessFile(), _errorFile(),
_accessStream(), _errorStream(),
_port(0), _address(INADDR_ANY), _clientMaxBodySize(0)
{
}

Config::~Config()
{
}

std::ofstream & Config::getAccessStream() const
{
	// return _accessStream;
	return & _accessStream;
}

std::ofstream & Config::getErrorStream() const
{
	// return _errorStream;
	return & _errorStream;
}

// uint16_t Config::getPort() const;
// uint32_t Config::getAddress() const;
// size_type Config::getClientMaxBodySize() const;

void Config::_createLogFile(const std::string accessFile, const std::string errorFile)
{
	std::string	file;
	if (accessFile.empty())
		file = "logs/access.log";
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
		file = "logs/error.log";
	else
		file = errorFile;
	_errorStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_errorStream.fail() == true)
	{
		std::cerr << "Error while creating error log's file: " << file << std::endl;
		exit(1);
	}
}
