#include "parse_config.hpp"

static void exitWithError(std::ostream & stream, const std::string message, const std::string line, int code)
{
	stream << message << line << std::endl;
	exit(code);
}

int tabLength(std::string *tab)
{
	int i = 0;
	while (!tab[i].empty())
		i++;
	return i;
}

static bool isNotBlank(std::string line)
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

static void openFile(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file", "" ,1);
	if (configStream.peek() == EOF)
		exitWithError(std::cerr, "Error: configuration file invalid: empty file", "", 1);
}

static void cleanConfig(std::string & buffer, std::ifstream & configStream)
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

int close_server_block(std::string & line, std::string & prevWord, bool *server_context, int *server_count)
{
	(void) prevWord;
	if (*server_context == false)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*server_context = false;
	(*server_count)++;	// DETERMINE LE NOMBRE D'OBJET CONFIG A AJOUTER AU SERVER
	std::cout << "\n";
	return VALID;
}

int open_location_block(std::string & line, std::string & prevWord, bool *location_context, int *location_count)
{
	(void) prevWord;
	(void) location_count;
	if (*location_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("location");
	std::string::iterator it(&line[pos + 7]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it) && (*it) != '{')
			return INVALID;
		if (*it == '{')
			*location_context = true;
	}
	return VALID;
}

int open_location_block_2(std::string & line, std::string & prevWord, bool *location_context, int *location_count)
{
	(void) location_count;
	if (*location_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("{");
	if (prevWord.compare(0, std::string::npos, "location", prevWord.length()) != EQUAL)
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
	*location_context = true;
	return VALID;
}

int open_location_block_3(std::string & line, std::string & prevWord, bool *location_context, int *location_count)
{
	(void) prevWord;
	(void) location_count;
	if (*location_context == true)
	{
		line = ERROR_SERVER_BLOCK;
		return INVALID;
	}
	int pos = line.find("location{");
	std::string::iterator it(&line[pos + 8]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*location_context = true;
	return VALID;
}

int close_location_block(std::string & line, std::string & prevWord, bool *location_context, int *location_count)
{
	(void) prevWord;
	if (*location_context == false)
		return INVALID;
	int pos = line.find("}");
	std::string::iterator it(&line[pos]);
	std::string::iterator ite = line.end();
	while (++it != ite)
	{
		if (!isblank(*it))
			return INVALID;
	}
	*location_context = false;
	(*location_count)++;	// DETERMINE LE NOMBRE D'OBJET CONFIG A AJOUTER AU SERVER
	std::cout << "\n";
	return VALID;
}

void parseConfig(std::string & configFile, Server & server)
{
	// ouverture du fichier
	std::ifstream configStream;
	openFile(configFile, configStream);

	// premiere passe sur le fichier: remplacant les ';'  par des '\n' et enleve les commentaires '# ...'
	std::string buffer;
	cleanConfig(buffer, configStream);

	// lecture et tri du buffer, ligne par ligne, mot par mot (variable word pour renseigner la config)
	std::string line, prevWord, word;
	std::istringstream iss_l(buffer), iss_w;
	int server_count = 0, location_count = 0, server_directive_index = -1, location_directive_index = -1;
	bool first_word = true, server_context = false, location_context = false, compare = false;
	f_ptr f_server_block[] = {&open_server_block, &open_server_block_2, &open_server_block_3, &close_server_block};
	f_ptr f_location_block[] = {&open_location_block, &open_location_block_2, &open_location_block_3, &close_location_block};
	std::string	server_block[] = {"server", "{", "server{", "}", ""};
	std::string location_block[] = {"location", "{", "location{", "}", ""};
	std::string	server_directives[] = {"listen", "server_name", "root", "index", "error_page", "client_max_body_size", ""};
	std::string	location_directives[] = {"root", "index", "methods", "autoindex", "redirection", "uploads_dir", "redirect", "cgi_bin", ""};
	int s_b = tabLength(server_block);
	int l_b = tabLength(location_block);
	int s_d = tabLength(server_directives);
	int l_d = tabLength(location_directives);
	while (std::getline(iss_l, line))				// lecture ligne par ligne
	{
		iss_w.str(line);
		while (isNotBlank(line) == true && iss_w)	// lecture mot par mot
		{
			iss_w >> word;
			if (word.length() == 0)
				break;
			if (first_word == true)
			{
				first_word = false;					
				for (int i = 0; compare == false && i < s_b; i++)
				{
					if (word.compare(0, std::string::npos, server_block[i].c_str(), word.length()) == EQUAL)
					{
						if (f_server_block[i](line, prevWord, &server_context, &server_count) == INVALID)
							exitWithError(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
					}
				}					
				for (int i = 0; compare == false && i < l_b; i++)
				{
					if (word.compare(0, std::string::npos, location_block[i].c_str(), word.length()) == EQUAL)
					{
						if (f_location_block[i](line, prevWord, &location_context, &location_count) == INVALID)
							exitWithError(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
					}
				}					
				for (int i = 0; compare == false && i < s_d; i++)
				{
					if (server_context == false)
					{	
						std::cout << "TEST"<< "\n";
						exitWithError(std::cerr, ERROR_MSG, line, 1);
					}
					if (word.compare(0, std::string::npos, server_directives[i].c_str(), word.length()) == EQUAL)
					{
						server_directive_index = i;
						std::cout << "S_DIRECTIVE: " << word << "\n";	// DIRECTIVE A AJOUTER DANS LES OBJETS CONFIGS
						compare = true;
					}
				}
				for (int i = 0; compare == false && i < l_d; i++)
				{
					if (server_context == false || location_context == false)
					{	
						std::cout << "TEST"<< "\n";
						exitWithError(std::cerr, ERROR_MSG, line, 1);
					}
					if (word.compare(0, std::string::npos, location_directives[i].c_str(), word.length()) == EQUAL)
					{
						location_directive_index = i;
						std::cout << "L_DIRECTIVE: " << word << "\n";	// DIRECTIVE A AJOUTER DANS LES OBJETS CONFIGS
						compare = true;
					}
				}
				if (compare == false)
				{
					std::cout << "TEST"<< "\n";	
					exitWithError(std::cerr, ERROR_MSG, line, 1);
				}
			}
			else
			{
				std::cout << "	" << word << "\n";	// ARGUMENT POUR LA DIRECTIVE (SERVER OU CONFIG, selon)
			}
			prevWord = line;
			word.clear();
		}
		iss_w.clear();
		first_word = true;
		compare = false;
		server_directive_index = -1;
		location_directive_index = -1;
	}
	if (server_context == true || location_context == true)
		exitWithError(std::cerr, ERROR_MSG, ERROR_SERVER_BLOCK, 1);
	configStream.close();
}
