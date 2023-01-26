#include "../includes/Location.hpp"

/* 
	************ CONST/DESTR
*/
Location::Location(std::string const & server_root, std::list<std::string> const & server_indexes) :
_route(),
_root(server_root),
_autoindex(false),
_indexFiles(std::list<std::string>(server_indexes)),
_defaultIndex(true),
_methods(std::list<std::string>(1, std::string(GET))),
_defaultMethods(true),
_uploadsDir(_root),
_redirections(std::list<Trio>()),
_contentType(std::list<std::string>())
{}

Location::Location() :
_route(),
_root(std::string(_webserv_bin_path().append("/").append(DEFAULT_ROOT))),
_autoindex(false),
_indexFiles(std::list<std::string>()),
_defaultIndex(true),
_methods(std::list<std::string>(1, std::string(GET))),
_defaultMethods(true),
_uploadsDir(_root),
_redirections(std::list<Trio>()),
_contentType(std::list<std::string>())
{}

Location::Location(const Location & src) :
_route(src._route),
_root(src._root),
_autoindex(src._autoindex), 
_indexFiles(std::list<std::string>(src._indexFiles)),
_defaultIndex(src._defaultIndex),
_methods(src._methods),
_defaultMethods(src._defaultMethods),
_uploadsDir(src._uploadsDir),
_redirections(src._redirections),
_cgi(src._cgi) 
{}

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
			set_uploadsdir(value);
			break;
		case I_REDIRECTION_L:
			set_redirection(value);
			break;
		case I_CONTENT:
			set_contentType(value);
			break;
	}
}

void Location::set_route(std::string value)
{
	// route value
	if (value.empty())
		return;
	_route = value;

	// // if route is a cgi, set cgi filename
	// if (value[0] != '/' )
	// 	_cgi = _root.append("/").append(get_indexes().front());
	// else if (value.back() != '/')
	// 	_root.append("/");
	// if route is a cgi, set cgi filename

	if (value[0] == '/' && value.back() != '/')
		_route.append("/");
}

void Location::set_root(std::string & value)
{
	if (value.empty())
		return;
	// si relative, on complete la partie qui précède pour uniformiser les path en absolute
	if (value[0] != '/')
		_root = _root.append("/").append(value);
	else
		_root = value;
	
	// toujours checker qu'il n'y ait pas de slash à la fin, question d'uniformité
	// quand on joint 2 paths, on sait qu'on doit toujours append("/") entre 2
	if (_root[_root.size() - 1] == '/')
		_root.erase(_root.size() - 1);
	_uploadsDir = _root;

	// if route is a cgi, set cgi filename
	if (_route[0] != '/')
	{
		_cgi = _root;
		_cgi.append("/").append(get_indexes().front());
	}
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
	{
		std::list<std::string>::iterator it = _methods.begin();
		for(; it !=_methods.end(); it++)
		{
			if (*it == value)
				return;
		}
		_methods.push_back(value);
	}
}

void Location::set_index(std::string & value)
{
	if (value.empty())
		return;
	if (value[0] == '/')
		value.erase(0, 1);
	if (_defaultIndex == true)
	{
		_indexFiles.clear();
		_indexFiles.push_back(value);
		_defaultIndex = false;
	}
	else
		_indexFiles.push_back(value);
	
	// if route is a cgi, set cgi filename
	if (_route[0] != '/')
	{
		_cgi = _root;
		_cgi.append("/").append(get_indexes().front());
	}
}

void Location::set_autoindex(std::string & value)
{
	if (value.empty())
		return;
	if (value.compare("on") == 0 || value.compare("true") == 0 || value.compare("1") == 0)
		_autoindex = true;
	else if (value.compare("off") == 0 || value.compare("false") == 0 || value.compare("0") == 0)
		_autoindex = false;
}

void Location::set_uploadsdir(std::string & value)
{
	if (value.empty())
		return;
	if (value[0] != '/')
		_uploadsDir = get_root().append("/").append(value);
	else
		_uploadsDir = value;
}

void Location::set_redirection(std::string & line)
{
	static std::string prev;
	if (line.empty())
		return;
	if (prev == line)
		return;

	Trio trio;
	std::istringstream iss(line);
	std::string word;
	int code = -1;
	iss >> word;
	while (iss)
	{
		if (iss.peek() == EOF)
			break;
		iss >> word;
		try {
			code = std::stoi(word);
			if (code > -1)
				trio.third = code;
		}
		catch (const std::invalid_argument &ia) {
			if (trio.first.empty())
			{
				if (word[0] == '/')
					word.erase(0, 1);
				trio.first = get_root().append("/").append(word);
			}
			else
			{
				if (word[0] != '/')
					trio.second = get_root().append("/").append(word);
				else
					trio.second = word;
			}
		}
	}
	prev = line;
	_redirections.push_back(trio);
}

void Location::set_contentType(std::string & value)
{
	if (value.empty())
		return;
	std::list<std::string>::iterator it = _contentType.begin();
	for(; it !=_contentType.end(); it++)
	{
		if (*it == value)
			return;
	}
	_contentType.push_back(value);
}

std::string Location::get_route() const { return _route; }

// path du directory
std::string Location::get_root() const { return _root; }

std::list<std::string> & Location::get_methods() { return _methods; }

// nom du/des fichier(s) d'index
std::list<std::string> & Location::get_indexes() { return _indexFiles; }

bool Location::get_autoindex() const { return _autoindex; }

// path du directory
std::string Location::get_uploadsdir() const { return _uploadsDir; }

std::list<Trio> & Location::get_redirections() { return _redirections; }

std::string Location::get_cgi() const { return _cgi; }

std::list<std::string> & Location::get_contentTypes() { return _contentType; }

std::string Location::_webserv_bin_path() const
{
	char * bin = getcwd(NULL, 0);
	std::string bin_str = bin;
	free(bin);
	return bin_str;
}
