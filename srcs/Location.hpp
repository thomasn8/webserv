#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <array>

# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define DEFAULT_CGI_PATH "cgi/"
# define DEFAULT_CGI_EXT "php"

class Location
{	
	
	public:
		typedef std::array<std::string, 3> old_new_status_tab;

		Location();
		~Location();

	private:
		std::string _prefix;	//   /images/
		int _prefixCount;

		std::string _root;
		const std::vector<std::string> & _indexFiles;
		bool _autoindex;

		int _methods;	// Get=4 + Post=2 + Delete=1
		std::string _uploadsDir;		

		// array[old_url, new_url, statusCode]
		 std::vector< old_new_status_tab >  _redirections;

		std::string _cgiBinPath;
		std::string _cgiExtension;
};

#endif
