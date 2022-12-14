#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <array>

# define DEFAULT_ROOT "www/html/"
# define DEFAULT_INDEX "index.html"
# define DEFAULT_CGI_PATH "cgi/"
# define DEFAULT_CGI_EXT "php"
# define GET 4
# define POST 2
# define DELETE 1

class Location
{
	public:
		typedef std::array<std::string, 3> old_new_status_tab;

		Location();
		Location(const Location & src);
		~Location();

		void addPrefix(std::string prefix);

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
