#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config(const std::string file) :
_configFile(file), _configStream(),
_accessFile(), _errorFile(),
_accessStream(), _errorStream(),
_serverNames(1, DEFAULT_IP), _address(INADDR_ANY), 
_port(DEFAULT_PORT), _clientMaxBodySize(MBS)
{
	_configStream.open(_configFile, std::ifstream::in);
	if (_configStream.fail() == true)
		_exitWithError(std::cerr, "Error while opening configuration file\n", 1);
	
	// parsing de _configStream
	// ...
	// if ( myfile.is_open() ) 
	// {
	// 	while ( myfile.good() ) 
	// 	{
	// 		myfile >> mystring;
	// 		std::cout << mystring;
	// 	}   
	// }

	std::string line;
	std::string str;
	while (_configStream)
	{
		std::getline(_configStream, line);
		while (line)
		line >> 
		// std::cout << line << "\n";
	
		line.clear();
	}
	_configStream.close();

	std::string accessFile;
	std::string errorFile;

	// _serverNames[0] = ...;
	// if (_serverNames[0]  == "localhost")
	// 	_serverNames[0]  == "127.0.0.1";
	// _address = ntohl(_serverNames[0] .c_str());
	// if (_adress == -1)
	// 	_exitWithError(std::cerr, "Error: Invalid IPv4 address\n", 1);
	
	_createLogFile(accessFile, errorFile);
}

Config::~Config()
{
	_accessStream.close();
	_errorStream.close();
}

/* 
	************ GETTERS
*/
std::ofstream & Config::getAccessStream()
{
	return _accessStream;
}

std::ofstream & Config::getErrorStream()
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

size_t Config::getClientMaxBodySize() const
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
		file = "conf/logs/access.log";
	else
		file = accessFile;
	_accessStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_accessStream.fail() == true)
		_exitWithError(std::cerr, "Error while creating access log file\n", 1);
	file.clear();
	if (errorFile.empty())
		file = "conf/logs/error.log";
	else
		file = errorFile;
	_errorStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_errorStream.fail() == true)
		_exitWithError(std::cerr, "Error while creating error log file\n", 1);
}

void Config::_exitWithError(std::ostream & stream, const std::string message, int code) const
{
	stream << message;
	exit(code);
}
