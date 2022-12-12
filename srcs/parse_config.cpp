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
	*server_block = true;
	return VALID;
}

int check_close_server_block(std::string line, bool *server_block)
{
	if (*server_block == false)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_block = false;
	return VALID;
}

void openFile(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file", -1 ,1);
	if (configStream.peek() == EOF)
		exitWithError(std::cerr, "Error: configuration file invalid: empty file", -1, 1);
}

void clearFile(std::string & buffer, std::ifstream & configStream)
{
	std::string line;
	char **str_split; 
	while (configStream)
	{
		std::getline(configStream, line);
		str_split = split_quotes(line.c_str(), ';');
		line.clear();
		for (int i = 0; str_split[i] != NULL; i++)
		{
			if (strcmp(str_split[i], ";") != 0)
				line += str_split[i];
			line += '\n';
		}		
		free_arr(str_split);
		str_split = NULL;
		str_split = split_quotes(line.c_str(), '#');
		if (str_split[0] != NULL && *str_split[0])
			buffer += str_split[0];
		buffer += '\n';
		free_arr(str_split);
		str_split = NULL;
	}
}

std::string get_next_line(std::string buffer)
{
	std::string line;
	// ...code here
	return line;
}

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs)
{
	// ouverture du fichier
	std::ifstream configStream;
	openFile(configFile, configStream);

	// premiere passe sur le fichier en remplacant tous les ';' pas entre guillemets par des '\n'
	// et pour enlever les commentaires '#...'
	std::string buffer;
	clearFile(buffer, configStream);

	// lecture du buffer, ligne par ligne, mot par mot
	std::string line, word;
	std::istringstream iss;
	int pos, line_num = 0;
	bool first_word = true;
	bool server_block = false;
	int server_count = 0;
	int directive_index = -1;
	std::string	g_directives[2] = {"keepalive_timeout", "server"};
	std::string	s_directives[9] = {"listen", "server_name", "method", "root", "index", "access_log", "error_log", "error_page", "client_max_body_size"};
	line = get_next_line(buffer);
	while (!line.empty())			// lecture line by line
	{
		line_num++;
		iss.str(line);
		while (iss)					// lecture mot par mot
		{
			iss >> word;
			if (word.length() == 0)
				break;
			std::cout << word << " ";
			if (first_word == true)				// DIRECTIVE
			{
				first_word = false;
				if (word.compare(0, std::string::npos, "keepalive_timeout", word.length()) == EQUAL)		// KEEPALIVE_TIMEOUT
				{
					// modifier le server timeout
				}
				else if (word.compare(0, std::string::npos, "server", word.length()) == EQUAL)				// SERVER BLOCK OPEN
				{
					if (check_open_server_block(line, &server_block) == INVALID)
						exitWithError(std::cerr, "1.Error: configuration file invalid: line ", line_num, 1);
					server_count++;
				}
				else if (word.compare(0, std::string::npos, "}", word.length()) == EQUAL)					// SERVER BLOCK CLOSE
				{
					if (check_close_server_block(line, &server_block) == INVALID)
						exitWithError(std::cerr, "1.Error: configuration file invalid: line ", line_num, 1);
				}
				else							// SERVER DIRECTIVE
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
			word.clear();
		}
		iss.clear();
		line.clear();
		line = get_next_line(buffer);
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
