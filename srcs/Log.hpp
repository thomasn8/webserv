#ifndef LOG_HPP
# define LOG_HPP

#include <string>
#include <iostream>
#include <fstream>

class Log
{	
	public:
		Log(const std::string accessFile, const std::string errorFile);
		~Log();
	
	private:
		Log();
		
		const std::string _accessFile;
		const std::string _errorFile;
		std::ofstream	_accessStream;
		std::ofstream	_errorStream;
};

#endif
