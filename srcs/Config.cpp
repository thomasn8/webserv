#include "Config.hpp"

/* 
	************ CONST/DESTR
*/
Config::Config() :
_accessFile(), _errorFile(),
_accessStream(), _errorStream(),
_serverNames(1, DEFAULT_IP), _address(INADDR_ANY), 
_port(DEFAULT_PORT), _clientMaxBodySize(MBS)
{
	// si specifier dans config, creer des log files specifiques dedie pour le server en question
	// _createLogFile(accessFile, errorFile);
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
void Config::_exitWithError(std::ostream & stream, const std::string message, int code) const
{
	_log(stream, message);
	exit(code);
}

std::ostream & Config::_log(std::ostream & stream, const std::string message) const
{
	stream << message;
	return stream;
}
