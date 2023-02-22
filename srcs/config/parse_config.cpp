#include "parse_config.hpp"

static void open_file(std::string & configFile, std::ifstream & configStream)
{
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		p_exit_cerr_msg("Error while opening configuration file", "" ,1);
	if (configStream.peek() == EOF)
		p_exit_cerr_msg("Error: configuration file invalid: empty file", "", 1);
}

/* premiere passe sur le fichier pour supporter les tokens: {};#
ajoute des '\n' autour des brackets, remplace les ';' par des '\n' et enleve les parties commentee '# ...'
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
		if (p_isnotblank(line))
		{
			buffer += line;
			if (line.back() != '\n')
				buffer += '\n';
		}
		if (configStream.peek() == EOF)
			break;
	}
}

void parse_config(std::string & configFile, Monitor & monitor)
{
	std::ifstream configStream;
	open_file(configFile, configStream);
	std::string buffer;
	clean_config(buffer, configStream);
	configStream.close();

	// lecture et tri du buffer: 
	// variables word et route utilisees pour remplir les objets Config et Location
	std::string line, word, route;
	std::istringstream iss_l(buffer), iss_w;
	int server_count = 0, location_count = 0, server_directive_index = -1, location_context = 0, location_directive_index = -1, ret = 0;
	bool first_word = true, server_context = false, compare = false;
	f_ptr_s f_server_block[] = {&open_server_block, &open_server_block_2, &close_server_block};
	f_ptr_l f_location_block[] = {&open_location_block, &open_location_block_2, &close_location_block};
	const std::string	server_block[] = {"server", "{", "}", ""};
	const std::string 	location_block[] = {"location", "{", "}", ""};
	int s_b = p_tablen(server_block);
	int l_b = p_tablen(location_block);
	int s_d = p_tablen(g_server_directives);
	int l_d = p_tablen(g_location_directives);
	while (std::getline(iss_l, line))				// lecture du buffer, ligne par ligne
	{
		iss_w.str(line);
		while (p_isnotblank(line) == true && iss_w)	// lecture de la ligne, mot par mot
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
					if (word == server_block[i])
					{
						if (f_server_block[i](line, &server_context, &server_count) == INVALID)
							p_exit_cerr_msg(ERROR_MSG, line, 1);
						if (i == 0)
							iss_w >> word;
						compare = true;
						if (server_context == true)
							monitor.add_server();
					}
				}
				// DETECTE LES BLOCK LOCATION			
				for (int i = 0; compare == false && i < l_b; i++)
				{
					if (word == location_block[i])
					{
						if ((ret = f_location_block[i](line, route, &location_context, &location_count)) >= INVALID)
						{
							if (ret == ROUTE_SYNTAX_INVALID)
								p_exit_cerr_msg(ERROR_LOCATION_SYNTAX, line, 1);
							else
								p_exit_cerr_msg(ERROR_MSG, line, 1);
						}
						if (i == 0)
							iss_w >> word;
						compare = true;
						if (location_context == 2)
						{
							monitor.get_servers().back().add_location(route);
							monitor.get_servers().back().get_last_location().set_route(route);
						}
					}
				}
				// DETECTE LES DIRECTIVES DE SERVER
				for (int i = 0; compare == false && location_context == false && i < s_d; i++)
				{
					if (server_context == false)
						p_exit_cerr_msg(ERROR_MSG, line, 1);
					if (word == g_server_directives[i])
					{
						server_directive_index = i;
						compare = true;
					}
				}
				// DETECTE LES DIRECTIVES DE LOCATION
				for (int i = 0; compare == false &&  i < l_d; i++)
				{
					if (server_context == false || location_context != 2)
						p_exit_cerr_msg(ERROR_MSG, line, 1);
					if (word == g_location_directives[i])
					{
						location_directive_index = i;
						compare = true;
					}
				}
				if (compare == false)
					p_exit_cerr_msg(ERROR_MSG, line, 1);
			}
			// DETECTE LES ARGUMENTS DE DIRECTIVES
			else
			{
				if (location_context == false)
				{
					if (server_directive_index == I_ERROR_PAGE_C && p_error_page_syntax(line) == false)
						p_exit_cerr_msg(ERROR_MSG, line, 1);
					monitor.get_servers().back().add_directive(server_directive_index, word);
				}
				else
				{
					if (location_directive_index == I_METHODS_L && p_method_syntax(word) == false)
						p_exit_cerr_msg(ERROR_MSG, line, 1);
					else if (location_directive_index == I_REDIRECTION_L && p_redirect_syntax(line) == false)
						p_exit_cerr_msg(ERROR_MSG, line, 1);
					if (location_directive_index != I_REDIRECTION_L)
						monitor.get_servers().back().get_last_location().add_directive(location_directive_index, word);
					else
						monitor.get_servers().back().get_last_location().add_directive(location_directive_index, line);
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
	if (location_context != BRACKET_CLOSED)
		p_exit_cerr_msg(ERROR_MSG, ERROR_LOCATION_BLOCK, 1);
	if (server_context != BRACKET_CLOSED)
		p_exit_cerr_msg(ERROR_MSG, ERROR_SERVER_BLOCK, 1);
	monitor.log(monitor.get_time(), " CONFIGURATION\n\n");
	monitor.log_server_info();
}
