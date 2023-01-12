#include "../../includes/Monitor.hpp"

void Monitor::log_server_info()
{
	_accessStream << "Nombre de server: " << get_servers().size() << std::endl;
	std::deque<Server>::iterator it = get_servers().begin();
	int i = 0;
	for (; it != get_servers().end(); it++)
	{
		//SERVER BLOCK
		i++;
		_accessStream << "SERVER #" << i << std::endl;
		_accessStream << "	" << "port: " << (*it).get_port_str() << std::endl;
		char ip4[INET_ADDRSTRLEN];
		_accessStream << "	" << "ip: " << (*it).get_ipv4_str() << std::endl;
		std::list<std::string>::iterator it7 = (*it).get_servernames().begin();
		for (; it7 != (*it).get_servernames().end(); it7++)
			_accessStream << "	" << "server_name: " << (*it7) << std::endl;
		_accessStream << "	" << "root: " << (*it).get_root() << std::endl;
		std::list<std::string>::iterator it6 = (*it).get_indexes().begin();
		for (; it6 != (*it).get_indexes().end(); it6++)
			_accessStream << "	" << "index: " << (*it6) << std::endl;
		std::list<Server::error_page_pair>::iterator it8 = (*it).get_errorpages().begin();
		for (; it8 != (*it).get_errorpages().end(); it8++)
		{
			_accessStream << "	" << "error_page: ";
			_accessStream << (*it8).first;
			_accessStream << ": ";
			_accessStream << (*it8).second;
			_accessStream << std::endl;
		}
		_accessStream << "	" << "client_mbs: " << (*it).get_client_max_body_size() << std::endl;
		std::deque<Location>::iterator it2 = (*it).get_locations().begin();
		for (; it2 != (*it).get_locations().end(); it2++)
		{	
			//LOCATION BLOCK OF SERVER
			_accessStream << "	" << "Location " << (*it2).get_route() << std::endl;
			_accessStream << "		" << "root: " << (*it2).get_root() << std::endl;
			std::list<std::string>::iterator it3 = (*it2).get_methods().begin();
			for (; it3 != (*it2).get_methods().end(); it3++)
				_accessStream << "		" << "method: " << (*it3) << std::endl;
			if ((*it2).get_autoindex() == true)
				_accessStream << "		" << "autoindex: " << "on" << std::endl;
			else
				_accessStream << "		" << "autoindex: " << "off" << std::endl;
			std::list<std::string>::iterator it4 = (*it2).get_indexes().begin();
			for (; it4 != (*it2).get_indexes().end(); it4++)
				_accessStream << "		" << "index: " << (*it4) << std::endl;
			_accessStream << "		" << "uploads_dir: " << (*it2).get_uploadsdir() << std::endl;
			if ((*it2).get_redirections().size())
			{
				std::list<Trio>::iterator it5 = (*it2).get_redirections().begin();
				for (; it5 != (*it2).get_redirections().end(); it5++)
					_accessStream << "		" << "redirect: " << (*it5).first << " " << (*it5).second << " " << (*it5).third << std::endl;
			}
			// _accessStream << "		" << "cgi: " << (*it2).get_cgiBinPath() << std::endl;
			std::list<std::string>::iterator it9 = (*it2).get_cgi().begin();
			for (; it9 != (*it2).get_cgi().end(); it9++)
				_accessStream << "		" << "cgi: " << (*it9) << std::endl;
		}
	}
	_accessStream << std::endl;
}
