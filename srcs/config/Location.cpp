#include "Location.hpp"

/* 
	************ CONST/DESTR
*/
Location::Location() :
_prefix(),
_prefixLevelCount(),
_root(std::string(DEFAULT_ROOT)),
_autoindex(false), 
_index(std::string(DEFAULT_INDEX)),
_methods(std::vector<std::string>(1, std::string(GET))),
_defaultMethods(true),
_uploadsDir(),
_redirections(std::vector< old_new_status_tab >()),
_cgiBinPath(std::string(DEFAULT_CGI_PATH)),
_cgiExtension(std::string(DEFAULT_CGI_EXT)) 
{}

Location::Location(const Location & src) :
_prefix(),
_prefixLevelCount(),
_root(std::string(DEFAULT_ROOT)),
_autoindex(false), 
_index(std::string(DEFAULT_INDEX)),
_methods(std::vector<std::string>(1, std::string(GET))),
_defaultMethods(true),
_uploadsDir(),
_redirections(std::vector< old_new_status_tab >()),
_cgiBinPath(std::string(DEFAULT_CGI_PATH)),
_cgiExtension(std::string(DEFAULT_CGI_EXT)) 
{ (void) src; }

Location::~Location() {}

/* 
	************ GETTERS/SETTERS
*/
void Location::add_directive(int directiveIndex, std::string value)
{
	switch (directiveIndex) 
	{
		case I_ROOT_L:
			set_root(value);
			break;
		case I_METHODS_L:
			set_method(value);
			break;
		case I_AUTOINDEX_L:
			set_autoindex(value);
			break;
		case I_INDEX_L:
			set_index(value);
			break;
		case I_UPLOADS_DIR_L:
			set_uploaddir(value);
			break;
		case I_REDIRECTION_L:
			set_redirection(value);
			break;
		case I_CGI_BIN_L:
			set_cgiBinPath(value);
			break;
	}
}

void Location::set_prefix(std::string value)
{
	if (value.empty())
		return;
	_prefix = value;
	if (value[0] == '.')
		_cgiExtension = value.substr(1);
	else if (value[0] == '*')
		_cgiExtension = value.substr(2);
}

void Location::set_root(std::string & value)
{
	if (value.empty())
		return;
	_root = value;
}

void Location::set_method(std::string & value)
{
	if (value.empty())
		return;
	if (_defaultMethods == true)
	{
		_methods.pop_back();
		_methods.push_back(value);
		_defaultMethods = false;
	}
	else
		_methods.push_back(value);
}

void Location::set_autoindex(std::string & value)
{
	if (value.empty())
		return;
	if (value.compare("on") == 0)
		_autoindex = true;
	else if (value.compare("off") == 0)
		_autoindex = false;
}

void Location::set_index(std::string & value)
{
	if (value.empty())
		return;
	_index = value;
}

void Location::set_uploaddir(std::string & value)
{

}

void Location::set_redirection(std::string & value)
{

}

void Location::set_cgiBinPath(std::string & value)
{

}

std::string Location::get_prefix() const { return _prefix; }

std::string Location::get_root() const { return _root; }

std::vector<std::string> & Location::get_methods() { return _methods; }

bool Location::get_autoindex() const { return _autoindex; }

std::string Location::get_index() const { return _index; }
