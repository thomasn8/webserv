#include "Location.hpp"

/* 
	************ CONST/DESTR
*/
Location::Location() :
_prefix(), _prefixLevelCount(),
_root(std::string(DEFAULT_ROOT)), _indexFiles(std::vector<std::string>(1, DEFAULT_INDEX)), _autoindex(false), 
_methods(GET), _uploadsDir(),
_redirections(std::vector< old_new_status_tab >()),
_cgiBinPath(std::string(DEFAULT_CGI_PATH)), _cgiExtension(std::string(DEFAULT_CGI_EXT)) 
{}

Location::Location(const Location & src) :
_prefix(), _prefixLevelCount(),
_root(std::string(DEFAULT_ROOT)), _indexFiles(std::vector<std::string>(1, DEFAULT_INDEX)), _autoindex(false), 
_methods(GET), _uploadsDir(),
_redirections(std::vector< old_new_status_tab >()),
_cgiBinPath(std::string(DEFAULT_CGI_PATH)), _cgiExtension(std::string(DEFAULT_CGI_EXT)) 
{ (void) src; }

Location::~Location() {}

/* 
	************ GETTERS/SETTERS
*/
// parse config
void Location::addPrefix(std::string prefix)
{
	_prefix = prefix;
	if (prefix[0] == '.')
		_cgiExtension = prefix.substr(1);
	else if (prefix[0] == '*')
		_cgiExtension = prefix.substr(2);
}

// ...
