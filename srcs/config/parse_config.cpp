#include "parse_config.hpp"

static void openFile(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file", "" ,1);
	if (configStream.peek() == EOF)
		exitWithError(std::cerr, "Error: configuration file invalid: empty file", "", 1);
}

/* premiere passe sur le fichier pour supporter les tokens: {};#
ajoute des '\n' autour des brackets, remplace les ';' par des '\n' et enleve les commentaires '# ...'
!!! ne prend pas en charge les guillemets */
static void cleanConfig(std::string & buffer, std::ifstream & configStream)
{
	std::string line;
	const char *token = "{};#";
	int pos = 0;
	while (configStream)
	{
		std::getline(configStream, line);
		pos = line.find_first_of(token, pos);
		while (pos != -1)
		{
			if (line[pos] == '{' || line[pos] == '}')
			{
				line.insert(pos++, "\n");
				line.insert(pos++ + 1, "\n");
			}
			else if (line[pos] == ';')
				line.replace(pos, 1, "\n");
			else if (line[pos] == '#')
				line.erase(pos);
			pos = line.find_first_of(token, pos);
		}
		pos = 0;
		if (isNotBlank(line))
		{
			buffer += line;
			if (line.back() != '\n')
				buffer += '\n';
		}
		if (configStream.peek() == EOF)
			break;
	}
}

void parseConfig(std::string & configFile, Server & server)
{
	std::ifstream configStream;
	openFile(configFile, configStream);
	std::string buffer;
	cleanConfig(buffer, configStream);

	// lecture et tri du buffer: 
	// variables word et prefix utilisees pour remplir les objets Config et Location
	std::string line, word, prefix;
	std::istringstream iss_l(buffer), iss_w;
	int server_count = 0, location_count = 0, server_directive_index = -1, location_context = 0, location_directive_index = -1;
	bool first_word = true, server_context = false, compare = false;
	f_ptr_s f_server_block[] = {&open_server_block, &open_server_block_2, &close_server_block};
	f_ptr_l f_location_block[] = {&open_location_block, &open_location_block_2, &close_location_block};
	std::string	server_block[] = {"server", "{", "}", ""};
	std::string location_block[] = {"location", "{", "}", ""};
	std::string	server_directives[] = {"listen", "server_name", "root", "index", "error_page", "client_max_body_size", ""};
	std::string	location_directives[] = {"root", "index", "methods", "autoindex", "redirection", "uploads_dir", "redirect", "cgi_bin", ""};
	int s_b = tabLength(server_block);
	int l_b = tabLength(location_block);
	int s_d = tabLength(server_directives);
	int l_d = tabLength(location_directives);
	while (std::getline(iss_l, line))				// lecture du buffer, ligne par ligne
	{
		iss_w.str(line);
		while (isNotBlank(line) == true && iss_w)	// lecture de la ligne, mot par mot
		{
			iss_w >> word;
			if (word.length() == 0)
				break;
			// DETECTE LES CONTEXTES (= BLOCK) ET LEURS DIRECTIVES
			if (first_word == true)
			{
				first_word = false;
				// DETECTE LES BLOCK SERVER
				for (int i = 0; compare == false && location_context == false && i < s_b; i++)
				{
					if (word.compare(0, std::string::npos, server_block[i].c_str(), word.length()) == EQUAL)
					{
						if (f_server_block[i](line, &server_context, &server_count) == INVALID)
							exitWithError(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
					}
				}
				// DETECTE LES BLOCK LOCATION			
				for (int i = 0; compare == false && i < l_b; i++)
				{
					if (word.compare(0, std::string::npos, location_block[i].c_str(), word.length()) == EQUAL)
					{
						if (f_location_block[i](line, prefix, &location_context, &location_count) == INVALID)
							exitWithError(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
						{
							iss_w >> word;
							std::cout << "L_PREFIX: " << prefix << "\n";	// A AJOUTER DANS LES OBJETS LOCATION
						}
						compare = true;
					}
				}
				// DETECTE LES DIRECTIVES DE SERVER
				for (int i = 0; compare == false && location_context == false && i < s_d; i++)
				{
					if (server_context == false)
						exitWithError(std::cerr, ERROR_MSG, line, 1);
					if (word.compare(0, std::string::npos, server_directives[i].c_str(), word.length()) == EQUAL)
					{
						server_directive_index = i;
						std::cout << "S_DIRECTIVE: " << word << "\n";		// A AJOUTER DANS LES OBJETS CONFIGS
						compare = true;
					}
				}
				// DETECTE LES DIRECTIVES DE LOCATION
				for (int i = 0; compare == false &&  i < l_d; i++)
				{
					if (server_context == false || location_context != 2)
						exitWithError(std::cerr, ERROR_MSG, line, 1);
					if (word.compare(0, std::string::npos, location_directives[i].c_str(), word.length()) == EQUAL)
					{
						location_directive_index = i;
						std::cout << "L_DIRECTIVE: " << word << "\n";		// A AJOUTER DANS LES OBJETS CONFIGS
						compare = true;
					}
				}
				if (compare == false)
					exitWithError(std::cerr, ERROR_MSG, line, 1);
			}
			// DETECTE LES ARGUMENTS DE DIRECTIVES
			else
			{
				std::cout << "	" << word << "\n";							// ARGUMENT POUR LA DIRECTIVE (SERVER OU LOCATION, selon)
			}
			word.clear();
			prefix.clear();
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
