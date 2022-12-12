#include "parse_config.hpp"

# define INVALID 1
# define VALID 0
# define EQUAL 0

void exitWithError(std::ostream & stream, const std::string message, int line_num, int code)
{
	if (line_num == -1)
		stream << message << std::endl;
	else
		stream << message << line_num << std::endl;
	exit(code);
}

int check_open_server_block(std::string line, bool *server_block)
{
	if (*server_block == true)
		return INVALID;
	int pos = line.find("server");
	std::string::iterator it(&line[pos + 5]); // iterator pointe sur le r de "server"
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it) && (*it) != '{')
			return INVALID;
	}
	*server_block == true;
	return VALID;
}

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs)
{
	// ouverture du fichier
	std::ifstream configStream;
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file", -1 ,1);
	
	std::string line, word;
	std::istringstream iss;
	int pos, line_num = 0;

	bool first_word = true;
	bool server_block = false;
	int server_count = 0;
	int directive_index = -1;
	std::string	g_directives[2] = {"keepalive_timeout", "server"};
	std::string	s_directives[9] = {"listen", "server_name", "method", "root", "index", "access_log", "error_log", "error_page", "client_max_body_size"};
	// std::cout << g_directives[1].length() << std::endl;
	if (configStream.peek() == EOF)
		exitWithError(std::cerr, "Error: configuration file invalid: empty file", -1, 1);
	while (configStream)			// lecture line by line
	{
		std::getline(configStream, line);
		line_num++;
		pos = line.find('#', 0);
		if (pos >= 0)
			line.erase(pos);
		// FAIRE DES OPERATION SUR LES LIGNES ICI
		// if (line.length())
		// 	std::cout << line << std::endl;
		iss.str(line);
		while (iss)					// lecture mot par mot
		{
			iss >> word;
			if (word.length() == 0)
				break;
			std::cout << word << " ";
			// FAIRE DES OPERATION SUR LES MOTS ICI
			if (first_word == true)				// DIRECTIVE
			{
				// std::cout << word << " ";
				first_word = false;
				if (word.compare(0, std::string::npos, "keepalive_timeout", word.length()) == EQUAL)		// KEEPALIVE_TIMEOUT
				{
					// modifier le server timeout
				}
				else if (word.compare(0, std::string::npos, "server", word.length()) == EQUAL)			// SERVER
				{
					if (check_open_server_block(line, &server_block) == INVALID)
						exitWithError(std::cerr, "1.Error: configuration file invalid: line ", line_num, 1);
					server_count++;
				}
				else											// SERVER DIRECTIVE
				{
					for (int i = 0; i < 9; i++)
					{
						if (word.compare(0, std::string::npos, s_directives[i].c_str(), word.length()) == EQUAL)
						{
							directive_index = i;
							break;
						}
					}
					if (directive_index == -1)
						exitWithError(std::cerr, "2.Error: configuration file invalid: line ", line_num, 1);
				}
			}
			else								// ARGUMENT DE DIRECTIVE
			{

			}

			// std::cout << word << " ";
			word.clear();
		}
		iss.clear();
		line.clear();
		first_word = true;
		directive_index = -1;
	}
	configStream.close();

	// std::string accessFile;
	// std::string errorFile;

	// _serverNames[0] = ...;
	// if (_serverNames[0]  == "localhost")
	// 	_serverNames[0]  == "127.0.0.1";
	// _address = ntohl(_serverNames[0] .c_str());
	// if (_adress == -1)
	// 	_exitWithError(std::cerr, "Error: Invalid IPv4 address\n", 1);	
}
