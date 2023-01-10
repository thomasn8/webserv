#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <sstream>
#include <list>
#include <unistd.h>

#include "Trio.hpp"

// INITIALIZATION
# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define DEFAULT_CGI_PATH "cgi_bin"
# define DEFAULT_CGI_EXT "php"
#include "methods.hpp"
# define DEFAULT_REDIRECT_CODE 301

// DIRECTIVE INDEX
// (pour en rajouter/modifier: modifier le tableau + definir une macro pour l'index + modifier le switch-case dans add_directive() et creer les getter/setter)
const std::string	g_location_directives[] = {"root", "method", "autoindex", "index", "uploads_dir", "redirect", "cgi_bin", ""};
# define I_ROOT_L 0
# define I_METHODS_L 1
# define I_AUTOINDEX_L 2
# define I_INDEX_L 3
# define I_UPLOADS_DIR_L 4
# define I_REDIRECTION_L 5
# define I_CGI_BIN_L 6

class Location
{
	public:
		// CONST/DESTR
		Location(std::string const & server_root, std::list<std::string> const & server_indexes);
		Location(const Location & src);
		~Location();
		
		// GETTERS/SETTERS
		void add_directive(int directiveIndex, std::string value);
		void set_prefix(std::string value);
		void set_root(std::string & value);
		void set_method(std::string & value);
		void set_index(std::string & value);
		void set_autoindex(std::string & value);
		void set_uploadsdir(std::string & value);
		void set_redirection(std::string & line);
		void set_cgiBinPath(std::string & value);
		std::string get_prefix() const;
		std::string get_root() const;
		std::list<std::string> & get_methods();
		std::string get_index() const;
		std::list<std::string> & get_indexes();
		bool get_autoindex() const;
		std::string get_uploadsdir() const;
		std::list<Trio> & get_redirections();
		std::string get_cgiBinPath() const;

	private:
		Location();
		std::string _prefix; // = route
		int _prefixLevelCount;
		std::string _root;
		std::list<std::string> _methods;
		bool _defaultMethods;
		bool _defaultIndex;
		std::list<std::string> _indexFiles;
		bool _autoindex;
		std::string _uploadsDir;
		std::list<Trio> _redirections;
		std::string _cgiBinPath;
		std::string _webserv_bin_path() const;
		std::string _cgiExtension;
};

#endif
