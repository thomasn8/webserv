#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <array>

// INITIALIZATION
# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define DEFAULT_CGI_PATH "cgi/"
# define DEFAULT_CGI_EXT "php"
# define GET 4
# define POST 2
# define DELETE 1

// DIRECTIVE INDEX
# define L_ROOT 0
# define L_INDEX 1
# define L_METHODS 2
# define L_AUTOINDEX 3
# define L_REDIRECTION 4
# define L_UPLOADS_DIR 5
# define L_REDIRECT 6
# define L_CGI_BIN 7

class Location
{
	public:
		typedef std::array<std::string, 3> old_new_status_tab;

		// CONST/DESTR
		Location();
		Location(const Location & src);
		~Location();
		
		// GETTERS/SETTERS
		// parse config
		void add_directive(int directiveIndex, std::string value);
		void add_prefix(std::string prefix);
		// sockets
		// ...

	private:
		std::string _prefix;	//   /images/
		int _prefixLevelCount;

		std::string _root;
		std::vector<std::string> _indexFiles;
		bool _autoindex;

		int _methods;
		std::string _uploadsDir;		

		// array[old_url, new_url, statusCode]
		std::vector< old_new_status_tab > _redirections;

		std::string _cgiBinPath;
		std::string _cgiExtension;
};

#endif
