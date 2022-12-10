#include "Log.hpp"

Log::Log(const std::string accessFile, const std::string errorFile) :
_accessFile(accessFile), _errorFile(errorFile)
{
	std::string	file;
	if (_accessFile.empty())
		file = "logs/access.log";
	else
		file = _accessFile;
	_accessStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_accessStream.fail() == true)
	{
		std::cerr << "Error while creating access log's file: " << file << std::endl;
		exit(1);
	}
	file.clear();
	if (_errorFile.empty())
		file = "logs/error.log";
	else
		file = _errorFile;
	_errorStream.open(file.c_str(), std::ofstream::out | std::ofstream::app);
	if (_errorStream.fail() == true)
	{
		std::cerr << "Error while creating error log's file: " << file << std::endl;
		exit(1);
	}
}

Log::~Log()
{
}

Log::Log()
{
}