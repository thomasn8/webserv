#include "parse_config.hpp"

static void open_file(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exit_with_error(std::cerr, "Error while opening configuration file", "" ,1);
	if (configStream.peek() == EOF)
		exit_with_error(std::cerr, "Error: configuration file invalid: empty file", "", 1);
}

/* premiere passe sur le fichier pour supporter les tokens: {};#
ajoute des '\n' autour des brackets, remplace les ';' par des '\n' et enleve les commentaires '# ...'
!!! ne prend pas en charge les guillemets */
static void clean_config(std::string & buffer, std::ifstream & configStream)
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
		if (is_not_blank(line))
		{
			buffer += line;
			if (line.back() != '\n')
				buffer += '\n';
		}
		if (configStream.peek() == EOF)
			break;
	}
}

void parse_config(std::string & configFile, Server & server)
{
	std::ifstream configStream;
	open_file(configFile, configStream);
	std::string buffer;
	clean_config(buffer, configStream);
	configStream.close();

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
	int s_b = tab_length(server_block);
	int l_b = tab_length(location_block);
	int s_d = tab_length(server_directives);
	int l_d = tab_length(location_directives);
	server.log(server.get_time(), " CONFIGURATION\n");
	while (std::getline(iss_l, line))				// lecture du buffer, ligne par ligne
	{
		iss_w.str(line);
		while (is_not_blank(line) == true && iss_w)	// lecture de la ligne, mot par mot
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
							exit_with_error(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
						if (server_context == true)
						{
							server.add_config();
							server.log("\nAdd a SERVER config\n");
						}
					}
				}
				// DETECTE LES BLOCK LOCATION			
				for (int i = 0; compare == false && i < l_b; i++)
				{
					if (word.compare(0, std::string::npos, location_block[i].c_str(), word.length()) == EQUAL)
					{
						if (f_location_block[i](line, prefix, &location_context, &location_count) == INVALID)
							exit_with_error(std::cerr, ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
						if (location_context == 2)
						{
							server.get_last_config().add_location();
							server.get_last_config().get_last_location().add_prefix(prefix);
							server.log("	Add a location: ", prefix, "\n");
						}
					}
				}
				// DETECTE LES DIRECTIVES DE SERVER
				for (int i = 0; compare == false && location_context == false && i < s_d; i++)
				{
					if (server_context == false)
						exit_with_error(std::cerr, ERROR_MSG, line, 1);
					if (word.compare(0, std::string::npos, server_directives[i].c_str(), word.length()) == EQUAL)
					{
						server_directive_index = i;
						server.log("	S_DIRECTIVE: ", word, "\n");
						compare = true;
					}
				}
				// DETECTE LES DIRECTIVES DE LOCATION
				for (int i = 0; compare == false &&  i < l_d; i++)
				{
					if (server_context == false || location_context != 2)
						exit_with_error(std::cerr, ERROR_MSG, line, 1);
					if (word.compare(0, std::string::npos, location_directives[i].c_str(), word.length()) == EQUAL)
					{
						location_directive_index = i;
						server.log("		L_DIRECTIVE: ", word, "\n");
						compare = true;
					}
				}
				if (compare == false)
					exit_with_error(std::cerr, ERROR_MSG, line, 1);
			}
			// DETECTE LES ARGUMENTS DE DIRECTIVES
			else
			{
				if (location_context == false)
				{
					server.get_last_config().add_directive(server_directive_index, word);
					server.log("		", word, "\n");
				}
				else
				{
					server.get_last_config().get_last_location().add_directive(location_directive_index, word);
					server.log("			", word, "\n");
				}
			}
			word.clear();
		}
		iss_w.clear();
		first_word = true;
		compare = false;
		server_directive_index = -1;
		location_directive_index = -1;
	}
	if (server_context == true || location_context == true)
		exit_with_error(std::cerr, ERROR_MSG, ERROR_SERVER_BLOCK, 1);
}
