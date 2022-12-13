#include "parse_config.hpp"

static void exitWithError(std::ostream & stream, const std::string message, const std::string line, int code)
{
	if (line.empty())
		stream << message << std::endl;
	else
		stream << message << line << std::endl;
	exit(code);
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
	(*server_count)++;	// DETERMINE LE NOMBRE D'OBJET CONFIG A AJOUTER AU SERVER
	std::cout << "\n";
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

	// lecture et tri du buffer, ligne par ligne, mot par mot (variable word pour renseigner la config)
	std::string line, prevWord, word;
	std::istringstream iss_l(buffer), iss_w;
	int server_count = 0, directive_index = -1;
	bool first_word = true, server_block = false, compare = false;
	f_ptr functions[] = {&check_open_server_block, &check_open_server_block_2, &check_open_server_block_3, &check_close_server_block};
	std::string	s_block[] = {"server", "{", "server{", "}"};
	std::string	s_directives[] = {"listen", "server_name", "method", "root", "index", "access_log", "error_log", "error_page", "client_max_body_size"};
	std::string	g_directives[] = {"keepalive_timeout", "server"};
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
				for (int i = 0; compare == false && i < 4; i++)
				{
					if (word.compare(0, std::string::npos, s_block[i].c_str(), word.length()) == EQUAL)
					{
						if (functions[i](line, prevWord, &server_block, &server_count) == INVALID)
							exitWithError(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
					}
				}					
				for (int i = 0; compare == false && i < 9; i++)
				{
					if (word.compare(0, std::string::npos, s_directives[i].c_str(), word.length()) == EQUAL)
					{
						directive_index = i;
						std::cout << "DIRECTIVE: " << word << "\n";	// DIRECTIVE A AJOUTER DANS LES OBJETS CONFIGS
						compare = true;
					}
				}
				if (compare == false && word.compare(0, std::string::npos, "keepalive_timeout", word.length()) == EQUAL)
				{
					std::cout << "DIRECTIVE: " << word << "\n";	// DIRECTIVE A AJOUTER DANS L'OBJET SERVER
					compare = true;
				}
				if (compare == false)
					exitWithError(std::cerr, ERROR_MSG, line, 1);
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
		directive_index = -1;
	}
	if (server_block == true)
		exitWithError(std::cerr, ERROR_MSG, ERROR_SERVER_BLOCK, 1);
	configStream.close();
}
