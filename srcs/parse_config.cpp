#include "parse_config.hpp"

# define INVALID 1
# define VALID 0
# define EQUAL 0
# define ERROR_MSG "Error: configuration file invalid:\n"
# define ERROR_SERVER_BLOCK "server{} directive error"

void exitWithError(std::ostream & stream, const std::string message, const std::string line, int code)
{
	if (line.empty())
		stream << message << std::endl;
	else
		stream << message << line << std::endl;
	exit(code);
}

bool isNotBlank(std::string line)
{
	std::string::iterator it = line.begin();
	std::string::iterator ite = line.end();
	while (it != ite)
	{
		if (!isblank(*it))
			return true;
		it++;
	}
	return false;
}

void openFile(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file", "" ,1);
	if (configStream.peek() == EOF)
		exitWithError(std::cerr, "Error: configuration file invalid: empty file", "", 1);
}

void cleanConfig(std::string & buffer, std::ifstream & configStream)
{
	std::string line, nocomment;
	char ***line_splitted = (char ***)malloc(sizeof(char ***));
	int i_first_split;
	while (configStream)
	{
		std::getline(configStream, line);
		split_quotes(line_splitted, line.c_str(), ';');
		line.clear();
		for (int i = 0; (*line_splitted)[i] != NULL; i++)
		{
			if (strcmp((*line_splitted)[i], ";") != 0)
				line += (*line_splitted)[i];
			line += '\n';
		}		
		free_arr((*line_splitted));
		(*line_splitted) = NULL;
		i_first_split = split_quotes(line_splitted, line.c_str(), '#');
		if ((*line_splitted)[0] != NULL && *(*line_splitted)[0])
		{
			nocomment = (*line_splitted)[0];
			if (i_first_split > -1)
				nocomment.erase(i_first_split, std::string::npos);
			buffer += nocomment;
			buffer += '\n';
		}
		free_arr((*line_splitted));
		(*line_splitted) = NULL;
		nocomment.clear();
	}
	free(line_splitted);
}

int check_open_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_block == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("server");
	std::string::iterator it(&line[pos + 5]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it) && (*it) != '{')
			return INVALID;
		if (*it == '{')
			*server_block = true;
	}
	return VALID;
}

int check_open_server_block_2(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) server_count;
	if (*server_block == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("{");
	if (prevWord.compare(0, std::string::npos, "server", prevWord.length()) != EQUAL)
	{
		line = prevWord;
		return INVALID;
	}
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_block = true;
	return VALID;
}

int check_open_server_block_3(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
	(void) server_count;
	if (*server_block == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("server{");
	std::string::iterator it(&line[pos + 6]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_block = true;
	return VALID;
}

int check_close_server_block(std::string & line, std::string & prevWord, bool *server_block, int *server_count)
{
	(void) prevWord;
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
	(*server_count)++;
	return VALID;
}

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs)
{
	// ouverture du fichier
	std::ifstream configStream;
	openFile(configFile, configStream);

	// premiere passe sur le fichier: remplacant les ';'  par des '\n' et enleve les commentaires '# ...'
	std::string buffer;
	cleanConfig(buffer, configStream);

	// lecture du buffer, ligne par ligne, mot par mot
	std::string line, prevWord, word;
	std::istringstream iss_l(buffer), iss_w;
	int server_count = 0, directive_index = -1;
	bool first_word = true, server_block = false;
	std::string	g_directives[2] = {"keepalive_timeout", "server"};
	std::string	s_block[4] = {"server", "{", "server{", "}"};
	f_ptr functions[4] = {&check_open_server_block, &check_open_server_block_2, &check_open_server_block_3, &check_close_server_block};
	std::string	s_directives[9] = {"listen", "server_name", "method", "root", "index", "access_log", "error_log", "error_page", "client_max_body_size"};
	while (std::getline(iss_l, line))			// lecture ligne par ligne
	{
		if (isNotBlank(line) == true)
		{
			iss_w.str(line);
			// std::cout << line << std::endl;
			while (iss_w)						// lecture mot par mot
			{
				iss_w >> word;
				if (word.length() == 0)
					break;
				// std::cout << word << " ";
				if (first_word == true)						// DIRECTIVE GENERAL
				{
					first_word = false;
					for (int i = 0; i < 4; i++)
					{
						if (word.compare(0, std::string::npos, s_block[i].c_str(), word.length()) == EQUAL)
						{
							if (functions[i](line, prevWord, &server_block, &server_count) == INVALID)
								exitWithError(std::cerr, ERROR_MSG, line, 1);
							if (i == 0)
								iss_w >> word;
							break;
						}
					}
					for (int i = 0; i < 9; i++)
					{
						if (word.compare(0, std::string::npos, s_directives[i].c_str(), word.length()) == EQUAL)
						{
							directive_index = i;
							std::cout << "1-" << word << "\n";
						}
						// else if (i == 8)
						// 	exitWithError(std::cerr, ERROR_MSG, line, 1);
					}
					if (word.compare(0, std::string::npos, "keepalive_timeout", word.length()) == EQUAL)
						std::cout << "1-" << word << "\n";
					// exitWithError(std::cerr, ERROR_MSG, line, 1);

					// first_word = false;
					// if (word.compare(0, std::string::npos, "keepalive_timeout", word.length()) == EQUAL)		// KEEPALIVE_TIMEOUT
					// {
					// 	std::cout << "1-" << word << "\n";
					// 	// modifier le server timeout
					// }
					// else if (word.compare(0, std::string::npos, "server", word.length()) == EQUAL)				// SERVER BLOCK OPEN
					// {
					// 	if (check_open_server_block(line, prevWord, &server_block, &server_count) == INVALID)
					// 		exitWithError(std::cerr, ERROR_MSG, line, 1);
					// 	iss_w >> word;
					// }
					// else if (word.compare(0, std::string::npos, "{", word.length()) == EQUAL)					// SERVER BLOCK CLOSE
					// {
					// 	if (check_open_server_block_2(line, prevWord, &server_block, &server_count) == INVALID)
					// 		exitWithError(std::cerr, ERROR_MSG, line, 1);
					// }
					// else if (word.compare(0, std::string::npos, "server{", word.length()) == EQUAL)				// SERVER BLOCK OPEN
					// {
					// 	if (check_open_server_block_3(line, prevWord, &server_block, &server_count) == INVALID)
					// 		exitWithError(std::cerr, ERROR_MSG, line, 1);
					// }
					// else if (word.compare(0, std::string::npos, "}", word.length()) == EQUAL)					// SERVER BLOCK CLOSE
					// {
					// 	if (check_close_server_block(line, prevWord, &server_block, &server_count) == INVALID)
					// 		exitWithError(std::cerr, ERROR_MSG, line, 1);
					// }
					// else									// DIRECTIVE SERVER
					// {
					// 	std::cout << "1-" << word << "\n";
					// 	for (int i = 0; i < 9; i++)
					// 	{
					// 		if (word.compare(0, std::string::npos, s_directives[i].c_str(), word.length()) == EQUAL)
					// 		{
					// 			directive_index = i;
					// 			break;
					// 		}
					// 	}
					// 	if (directive_index == -1)
					// 		exitWithError(std::cerr, ERROR_MSG, line, 1);
					// }
				}
				else										// ARGUMENT DE DIRECTIVE
				{
					std::cout << "2-" << word << "\n";
				}
				prevWord.clear();
				prevWord = line;
				word.clear();
			}
			iss_w.clear();
			first_word = true;
			directive_index = -1;
		}
		// prevWord.clear();
		// prevWord = line;
		line.clear();
	}
	configStream.close();
	if (server_block == true)
		exitWithError(std::cerr, ERROR_MSG, ERROR_SERVER_BLOCK, 1);
}
