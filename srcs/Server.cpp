#include "../includes/Server.hpp"

/* 
	************ CONST/DESTR
*/
Server::Server() :
_configs(std::deque<Config>()),
// _server_fd(), _client_fd(), _client_read(), 
// _address(), _buffer(), _response(),
_accessFile(std::string(LOG_PATH)), _accessStream()
{
	_create_log_file(_accessFile, _accessStream);
	
	
	// lancement du server
	// _server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// if (_server_fd < 0)
	// 	_exit_with_error(_config.getErrorStream(), "socket failed\n", EXIT_FAILURE);
	
	// _buffer = (char *)calloc(1, _config.get_client_max_body_size());
	// if (_buffer == NULL)
	// 	_exit_with_error(_config.getErrorStream(), "buffer alloc error\n", EXIT_FAILURE);

	// memset(_address.sin_zero, 0, sizeof(_address.sin_zero));
	// _address.sin_family = AF_INET;
	// _address.sin_addr.s_addr = htonl(_config.get_address());
	// _address.sin_port = htons(_config.get_port());
	// _address.sin_len = sizeof(_address);
	
	// if (bind(_server_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
	// 	_exit_with_error(_config.getErrorStream(), "bind failed\n", EXIT_FAILURE);
	// if (listen(_server_fd, LISTEN_BACKLOG) < 0)
	// 	_exit_with_error(_config.getErrorStream(), "listen failed\n", EXIT_FAILURE);
    // _response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!";


	// while(1)
	// {
	// 	std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
	// 	client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	// 	if (client_fd < 0)
	// 	{
	// 		perror("accept failed");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	client_read = read(client_fd, buffer, 30000);
	// 	if (client_read < 0)
	// 		perror("No bytes are there to read");
	// 	std::cout << buffer << std::endl;
	// 	write(client_fd, response.c_str(), response.length());
	// 	std::cout << "------- Hello message sent -------" << std::endl;
	// 	close(client_fd);
	// }
}

Server::~Server()
{
	_accessStream.close();
	// close(_server_fd);
	// free(_buffer);
}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Config> & Server::get_configs() { return _configs; }

Config & Server::get_last_config() { return get_configs().back(); }

void Server::add_config() { _configs.push_back(Config()); }

/* 
	************ SOCKETS
*/


/* 
	************ LOG
*/
void Server::_create_log_file(std::string const & filename, std::ofstream & stream)
{
	stream.open(filename, std::ofstream::out | std::ofstream::app);
	if (stream.fail() == true)
		_exit_cerr_msg("Error while creating access log file\n", 1);
}

void Server::_exit_cerr_msg(const std::string message, int code) const
{
	std::cerr << message;
	exit(code);
}

std::string Server::get_time()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80] = {0};
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void Server::log_config_info()
{
	_accessStream << "Nombre de server: " << get_configs().size() << std::endl;
	std::deque<Config>::iterator it = get_configs().begin();
	std::deque<Config>::iterator ite = get_configs().end();
	std::deque<Location>::iterator it2;
	std::deque<Location>::iterator it2e;
	int i = 0;
	for (; it != ite; it++)
	{
		i++;
		_accessStream << "SERVER #" << i << std::endl;
		_accessStream << "	" << "port: " << ntohs((*it).get_port()) << std::endl;
		_accessStream << "	" << "address: " << ntohl((*it).get_address()) << std::endl;
		for (int j = 0; j < (*it).get_servernames().size(); j++)
			_accessStream << "	" << "server_name: " << (*it).get_servernames()[j] << std::endl;
		_accessStream << "	" << "root: " << (*it).get_root() << std::endl;
		for (int j = 0; j < (*it).get_indexes().size(); j++)
			_accessStream << "	" << "index: " << (*it).get_indexes()[j] << std::endl;
		for (int j = 0; j < (*it).get_errorpages().size(); j++)
		{
			_accessStream << "	" << "error_page: ";
			_accessStream << (*it).get_errorpages()[j].first;
			_accessStream << ": ";
			_accessStream << (*it).get_errorpages()[j].second;
			_accessStream << std::endl;
		}
		_accessStream << "	" << "client_mbs: " << (*it).get_client_max_body_size() << std::endl;
		it2 = (*it).get_locations().begin();
		it2e = (*it).get_locations().end();
		for (; it2 != it2e; it2++)
		{	
			_accessStream << "	" << "Location " << (*it2).get_prefix() << std::endl;
			_accessStream << "		" << "root: " << (*it2).get_root() << std::endl;
			std::list<std::string>::iterator it3 = (*it2).get_methods().begin();
			std::list<std::string>::iterator ite3 = (*it2).get_methods().end();
			for (; it3 != ite3; it3++)
				_accessStream << "		" << "method: " << (*it3) << std::endl;
			if ((*it2).get_autoindex() == true)
				_accessStream << "		" << "autoindex: " << "on" << std::endl;
			else
				_accessStream << "		" << "autoindex: " << "off" << std::endl;
			_accessStream << "		" << "index: " << (*it2).get_index() << std::endl;
			_accessStream << "		" << "uploads_dir: " << (*it2).get_uploadsdir() << std::endl;
			if ((*it2).get_redirections().size())
			{
				std::list<Trio>::iterator it4 = (*it2).get_redirections().begin();
				std::list<Trio>::iterator ite4 = (*it2).get_redirections().end();
				for (; it4 != ite4; it4++)
					_accessStream << "		" << "redirect: " << (*it4).first << " " << (*it4).second << " " << (*it4).third << std::endl;
			}
			_accessStream << "		" << "cgi: " << (*it2).get_cgiBinPath() << std::endl;
		}
	}
}
