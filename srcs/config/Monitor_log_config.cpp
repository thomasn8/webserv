#include "../../includes/Monitor.hpp"

void Monitor::log_server_info()
{
	_log << get_time() << " CONFIGURATION\n" << std::endl;

	_log << "Nombre de server: " << get_servers().size() << std::endl;
	std::deque<Server>::iterator it = get_servers().begin();
	int i = 0;
	for (; it != get_servers().end(); it++)
	{
		//SERVER BLOCK
		i++;
		_log << "SERVER #" << i << std::endl;
		_log << "	" << "port: " << (*it).get_port_str() << std::endl;
		_log << "	" << "ip: " << (*it).get_ipv4_str() << std::endl;
		std::list<std::string>::const_iterator it7 = (*it).get_servernames().begin();
		for (; it7 != (*it).get_servernames().end(); it7++)
			_log << "	" << "server_name: " << (*it7) << std::endl;
		_log << "	" << "root: " << (*it).get_root() << std::endl;
		std::list<std::string>::const_iterator it6 = (*it).get_indexes().begin();
		for (; it6 != (*it).get_indexes().end(); it6++)
			_log << "	" << "index: " << (*it6) << std::endl;
		std::list<Server::error_page_pair>::const_iterator it8 = (*it).get_errorpages().begin();
		for (; it8 != (*it).get_errorpages().end(); it8++)
		{
			_log << "	" << "error_page: ";
			_log << (*it8).first;
			_log << ": ";
			_log << (*it8).second;
			_log << std::endl;
		}
		_log << "	" << "client_mbs: " << (*it).get_max_body_size() << std::endl;
		std::deque<Location>::const_iterator it2 = (*it).get_locations().begin();
		for (; it2 != (*it).get_locations().end(); it2++)
		{	
			//LOCATION BLOCK OF SERVER
			if ((*it2).get_cgi().empty())
				_log << "	" << "Location " << (*it2).get_route() << std::endl;
			else
				_log << "	" << "Location ." << (*it2).get_route() << std::endl;
			_log << "		" << "root: " << (*it2).get_root() << std::endl;
			std::list<std::string>::const_iterator it3 = (*it2).get_methods().begin();
			for (; it3 != (*it2).get_methods().end(); it3++)
				_log << "		" << "method: " << (*it3) << std::endl;
			std::list<std::string>::const_iterator it10 = (*it2).get_contentTypes().begin();
			for (; it10 != (*it2).get_contentTypes().end(); it10++)
				_log << "		" << "content: " << (*it10) << std::endl;
			if ((*it2).get_autoindex() == true)
				_log << "		" << "autoindex: " << "on" << std::endl;
			else
				_log << "		" << "autoindex: " << "off" << std::endl;
			std::list<std::string>::const_iterator it4 = (*it2).get_indexes().begin();
			for (; it4 != (*it2).get_indexes().end(); it4++)
				_log << "		" << "index: " << (*it4) << std::endl;
			_log << "		" << "uploads_dir: " << (*it2).get_uploadsdir() << std::endl;
			if ((*it2).get_redirections().size())
			{
				std::list<Trio>::const_iterator it5 = (*it2).get_redirections().begin();
				for (; it5 != (*it2).get_redirections().end(); it5++)
					_log << "		" << "redirect: " << (*it5).first << " " << (*it5).second << " " << (*it5).third << std::endl;
			}
			if ((*it2).get_cgi().size())
			_log << "		" << "cgi: " << (*it2).get_cgi() << std::endl;
		}
	}
	_log << std::endl;
}
