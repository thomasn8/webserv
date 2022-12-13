#include "Location.hpp"

/* 
	************ CONST/DESTR
*/
Location::Location() :
_prefix(), _prefixCount(),
_root(std::string(DEFAULT_ROOT)), _indexFiles(std::vector<std::string>(1, DEFAULT_INDEX)), _autoindex(false), 
_methods(4), _uploadsDir(std::string()),
_redirections(std::vector< old_new_status_tab >()),
_cgiBinPath(std::string(DEFAULT_CGI_PATH)), _cgiExtension(std::string(DEFAULT_CGI_EXT))
{

}

Location::~Location() {}
