#include "../includes/Monitor.hpp"
/* 
	************ CONST/DESTR
*/
Monitor::Monitor() :
_servers(std::deque<Server>()),
_master_sockets(std::vector<int>()),
_fd_count(0),
_fd_capacity(0),
_pfds(NULL),
_accessFile(std::string(LOG_PATH)), _accessStream()
{
	_create_log_file(_accessFile, _accessStream);
}

Monitor::~Monitor()
{
	// CLOSE ET FREE TOUT CE QUI FAUT DANS LA PARTIE SOCKET (array de pollfds)
	_accessStream.close();
}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Server> & Monitor::get_servers() { return _servers; }

Server & Monitor::get_last_server() { return get_servers().back(); }

void Monitor::add_server() { _servers.push_back(Server()); }

/* 
	************ SOCKETS ( !!! allocations -> free dans le destructeur)
*/
void Monitor::_prepare_master_sockets()
{
	it_servers it = _servers.begin();
	it_servers ite = _servers.end();
	int socket_fd;
	while (it != ite)
	{
		socket_fd = (*it).create_socket();
		log((*it).get_ip(), ":", ntohs((*it).get_address().sin_port), " listening on socket ", socket_fd, "\n");
		_master_sockets.push_back(socket_fd);
		_fd_count++;
		it++;
	}
	_fd_capacity = _fd_count + 5;
	_pfds = (struct pollfd *)malloc(sizeof(*_pfds) * _fd_capacity);
	_activeSockets = (struct socket *)malloc(sizeof(*_activeSockets) * _fd_capacity);
	if (_pfds == NULL)
	{
		log("Error: impossible to allocate", _fd_capacity ," struct pollfd\n");
		_exit_cerr_msg("Fatal error: allocation\n", 1);
	}
	for (int i = 0; i < _fd_count; i++)
	{
		_pfds[i].fd = _master_sockets[i];
    	_pfds[i].events = POLLIN;
    	_pfds[i].revents = 0;
		_activeSockets[i].pfd = NULL;
	}
	if (_servers.size() == 1)
		log(get_time(), " SERVER STARTED\n\n");
	else
		log(get_time(), " SERVERS STARTED\n\n");
}

void Monitor::_add_to_pfds(int new_fd, struct sockaddr_in * remoteAddr, void * server)
{
	if (_fd_count == _fd_capacity)
	{
		_fd_capacity *= 2;
		_pfds = (struct pollfd *)realloc(_pfds, sizeof(*_pfds) * (_fd_capacity));
		_activeSockets = (struct socket *)realloc(_activeSockets, sizeof(*_activeSockets) * (_fd_capacity));
		if (_pfds == NULL)
		{
			log("Error: impossible to allocate", _fd_capacity ," struct pollfd\n");
			_exit_cerr_msg("Fatal error: allocation\n", 1);
		}
	}
	_pfds[_fd_count].fd = new_fd;
	_pfds[_fd_count].events = POLLIN;
	_pfds[_fd_count].revents = 0;
	_activeSockets[_fd_count].pfd = &_pfds[_fd_count];
	_activeSockets[_fd_count].remoteAddr = *remoteAddr;
	_activeSockets[_fd_count].server = server;
	_fd_count++;
}

void Monitor::_del_from_pfds(int i)
{
	_pfds[i] = _pfds[_fd_count - 1];
	_activeSockets[i] = _activeSockets[_fd_count - 1];
	_fd_count--;
}

// AJOUTER UN TIMER POUR LES LOOP RECV ET SEND AU CAS OU LES FONCTIONS SONT BLOCKEES POUR BREAK ET ENVOYER UNE ERREUR
// VOIR POUR KEEP_ALIVE: CONSERVER LE SOCKET DU CLIENT APRES L'ENVOI D'UNE REPONSE (persistent-connection or keep-alive connection)
// A LA FIN SPLITER LA LOOP EN FONCTIONS INDIV
// VOIR SI LA HEAP GROSSI PAS A L'INFINI SANS LIBERE DE L'ESPACE CAR del_from_pfds() NE FREE PAS
void Monitor::handle_connections()
{
	_prepare_master_sockets(); // socket, bind, listen pour chaque port + creer les struct pollfd dédiées
	struct sockaddr_in remoteAddr;
	int i, poll_index = 0;
	int poll_count = 0, server_count = _servers.size();
	int new_fd = -1, polled_fd = -1;
	int size_recv = 0, total_recv = 0, size_sent = 0, total_sent = 0;
	char chunk_read[CHUNK_SIZE], chunk_send[CHUNK_SIZE];
	std::string request_recv;
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";
	while (1)													// Main loop
	{
		poll_count = poll(_pfds, _fd_count, -1);				// Sonde le nombre de pfd avec un event ready (blockant tant qu'il y en a aucun)
        if (poll_count < 0)
			log(get_time(), " Error: poll failed\n");
		i = poll_index;
		while (i < _fd_count)									// Run through the existing connections
		{
			if (_pfds[i].revents & POLLIN)						// We have data to read in the existing connections
			{
				polled_fd = _pfds[i].fd;
				for (int j = 0; j < server_count; j++)			// First, check only the master sockets
				{
					if (polled_fd == _master_sockets[j])		// Accept() new connection si fd correspond a un socket listening
					{
						remoteAddr.sin_len = sizeof(remoteAddr);
						new_fd = accept(_master_sockets[j], (struct sockaddr *)&remoteAddr, (socklen_t *)&remoteAddr.sin_len);
						if (new_fd < 0)
							log(get_time(), " Error: accept: new connexion on port ", ntohs(_servers[j].get_port()), " failed\n");
						else
						{
							_add_to_pfds(new_fd, &remoteAddr, &_servers[j]);
							std::cout << "Client " << inet_ntoa(remoteAddr.sin_addr) << ":" << ntohs(remoteAddr.sin_port) << " connected on server port " << ntohs(_servers[j].get_port()) << " via socket " << new_fd << std::endl;
							log(get_time(), " Client ", inet_ntoa(remoteAddr.sin_addr), ":", ntohs(remoteAddr.sin_port), " connected on server port ", ntohs(_servers[j].get_port()), " via socket ", new_fd, "\n");
						}
						break;
					}
					if (j == server_count - 1)					// If not a master socket so a client is ready to write
					{
						while (1)
						{
							size_recv = recv(polled_fd, chunk_read, CHUNK_SIZE, 0); // recv la request jusqu'au bout du client_fd
							std::cout << size_recv << " bytes read on socket " << polled_fd << std::endl;
							if (size_recv > 0)
								request_recv.append(chunk_read, size_recv);
							if (size_recv < CHUNK_SIZE) // toute la request a été read
							{
								std::cout << "Request from client " << inet_ntoa(_activeSockets[i].remoteAddr.sin_addr) << ":" << ntohs(_activeSockets[i].remoteAddr.sin_port) << " on server port ";
								std::cout << ntohs((static_cast<Server *>(_activeSockets[i].server))->get_port()) << " via socket " << _activeSockets[i].pfd->fd << std::endl;
								log(get_time(), " Request from client ", inet_ntoa(_activeSockets[i].remoteAddr.sin_addr), ":", ntohs(_activeSockets[i].remoteAddr.sin_port), " on server port ", ntohs((static_cast<Server *>(_activeSockets[i].server))->get_port()), " via socket ", _activeSockets[i].pfd->fd, "\n");
								try {
									Request request(request_recv.c_str());
									Response response(request, this->_servers[0]);

									// decomment to display in terminal:
									// std::cout << request.get_method() << " " << request.get_target() << " " << request.get_version() << std::endl;
									// request.display_fields();
									// std::cout << "\n" << request.get_body() << std::endl;

									// Response response(request);
								}
								catch (Request::MessageException & e) {
									std::cout << "Error: " << e.what() << std::endl;
								}
								request_recv.clear();
								_pfds[i].events = POLLOUT;
								poll_index = i;		// permet de revenir dans la loop infinie avec l'index du pfds où écrire
								j = server_count;	// break la for loop
								i = _fd_count;		// break la while loop
								break;
							}
						}
					}
				}
			}
			else if (_pfds[i].revents & POLLOUT)
			{
				const char * chunk_send = response.c_str();
				int response_size = response.size();
				if (response_size < CHUNK_SIZE)	// cas où response initiale fait < 512
				{
					size_sent = send(polled_fd, chunk_send, response_size, 0);
					std::cout << size_sent << " bytes sent on socket " << polled_fd << std::endl;
					response_size -= size_sent;
					chunk_send += size_sent;
					total_sent += size_sent;
				}
				while (response_size > CHUNK_SIZE) // cas où response initiale > 512
				{
					size_sent = send(polled_fd, chunk_send, CHUNK_SIZE, 0);
					std::cout << size_sent << " bytes sent on socket " << polled_fd << std::endl;
					response_size -= size_sent;
					chunk_send += size_sent;
					total_sent += size_sent;
				}
				while (response_size > 0) // cas où il reste des bytes a envoyer (envoyer les derniers bytes lorsque response initiale était > 512 bytes)
				{
					size_sent = send(polled_fd, chunk_send, response_size, 0);
					std::cout << size_sent << " bytes sent on socket " << polled_fd << std::endl;
					response_size -= size_sent;
					chunk_send += size_sent;
					total_sent += size_sent;
				}
				if (total_sent == response.size())
				{
					std::cout << "Response ("<< total_sent << ") bytes sent successfully on socket " << polled_fd << ", connection closed\n";
					log(get_time(), " Response successful: ", total_sent, " bytes sent on socket ", polled_fd, ", connection closed\n");
				}
				else
				{
					std::cout << "Error: response partially sent (" << total_sent << "/" << response.size() << ") on socket " << polled_fd << ", connection closed\n";
					log(get_time(), " Error: response partially sent (", total_sent, "/", response.size(), ") on socket ", polled_fd, ", connection closed\n");
				}
				close(polled_fd);
				_del_from_pfds(i);
				total_sent = 0;
				poll_index = 0;
				break;
			}
			i++;
		}
	}
}

/* 
	************ LOG
*/
void Monitor::_create_log_file(std::string const & filename, std::ofstream & stream)
{
	stream.open(filename, std::ofstream::out | std::ofstream::app);
	if (stream.fail() == true)
		_exit_cerr_msg("Error while creating access log file\n", 1);
}

void Monitor::_exit_cerr_msg(const std::string message, int code) const
{
	std::cerr << message;
	exit(code);
}

std::string Monitor::get_time()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80] = {0};
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void Monitor::log_server_info()
{
	_accessStream << "Nombre de server: " << get_servers().size() << std::endl;
	std::deque<Server>::iterator it = get_servers().begin();
	std::deque<Server>::iterator ite = get_servers().end();
	std::deque<Location>::iterator it2;
	std::deque<Location>::iterator it2e;
	int i = 0;
	for (; it != ite; it++)
	{
		i++;
		_accessStream << "SERVER #" << i << std::endl;
		_accessStream << "	" << "port: " << ntohs((*it).get_port()) << std::endl;
		char ip4[INET_ADDRSTRLEN];
		_accessStream << "	" << "ip: " << (*it).get_ip() << std::endl;
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
	_accessStream << std::endl;
}
