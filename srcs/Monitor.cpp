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
_activeSockets(NULL),
_accessFile(std::string(LOG_PATH)), _accessStream()
{
	_create_log_file(_accessFile, _accessStream);
}

Monitor::~Monitor()
{
	for (int i = 0; i < _fd_count; i++)
		close(_pfds[i].fd);
	if (_pfds)
		free(_pfds);
	if (_activeSockets)
		free(_activeSockets);
	_accessStream.close();
	log(get_time(), " Server shut down\n");
	std::cout << "Server shut down" << std::endl;
}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Server> & Monitor::get_servers() { return _servers; }

Server & Monitor::get_last_server() { return get_servers().back(); }

void Monitor::add_server() { _servers.push_back(Server()); }

/* 
	************ SOCKETS
*/
void Monitor::_prepare_master_sockets()
{
	it_servers it = _servers.begin();
	it_servers ite = _servers.end();
	int socket_fd;
	while (it != ite)
	{
		socket_fd = (*it).create_socket();
		log((*it).get_ipv4_port_str(), " listening on socket ", socket_fd, "\n");
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

struct socket * Monitor::_add_to_pfds(int new_fd, struct sockaddr_in * remoteAddr, Server * server)
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
	struct socket * activeSocket = &_activeSockets[_fd_count];
	activeSocket->pfd = &_pfds[_fd_count];
	activeSocket->remoteAddr = *remoteAddr;
	activeSocket->server = server;
	_fd_count++;

	// renseigne l'adresse du client pour eviter de multiplier les appels à inet_ntoa() et ntohs() dans les logs
	int i = 0;
	const char * ip = inet_ntoa(activeSocket->remoteAddr.sin_addr);
	const char * port = std::to_string(ntohs(activeSocket->remoteAddr.sin_port)).c_str();
	while (ip[i])
	{
		activeSocket->client[i]  = ip[i];
		i++;
	}
	activeSocket->client[i++] = ':';
	int j = 0;
	while (port[j])
	{
		activeSocket->client[i] = port[j];
		i++;
		j++;
	}
	while (i < 21)
	{
		activeSocket->client[i] = 0;
		i++;
	}
	return activeSocket;
}

void Monitor::_del_from_pfds(int i)
{
	_pfds[i] = _pfds[_fd_count - 1];
	_activeSockets[i] = _activeSockets[_fd_count - 1];
	_fd_count--;
}

void Monitor::_accept_new_connection(int master_index)
{
	struct sockaddr_in remoteAddr;
	remoteAddr.sin_len = sizeof(remoteAddr);
	int new_fd = accept(_master_sockets[master_index], (struct sockaddr *)&remoteAddr, (socklen_t *)&remoteAddr.sin_len);
	if (new_fd < 0)
		log(get_time(), " Error: accept: new connexion on port ", _servers[master_index].get_port_str(), " failed\n");
	else
	{
		struct socket * client_socket = _add_to_pfds(new_fd, &remoteAddr, &_servers[master_index]);
		log(get_time(), " New connection  ", client_socket->client, " on server port ", _servers[master_index].get_port_str(), " via socket ", new_fd, "\n");
	}
}

int Monitor::_recv_all(int fd, std::string & request, struct socket & activeSocket)
{
	char chunk_read[CHUNK_SIZE];
	int size_recv = 0, total_recv = 0;
	while (1)
	{
		size_recv = recv(fd, chunk_read, CHUNK_SIZE, 0); // recv la request jusqu'au bout du client_fd
		total_recv += size_recv;
		// std::cout << size_recv << " bytes read on socket " << fd << std::endl;
		if (size_recv > 0)
			request.append(chunk_read, size_recv);
		if (size_recv < CHUNK_SIZE) // toute la request a été read
		{
			log(get_time(), " Request from    ", activeSocket.client, " on server port ", activeSocket.server->get_port_str(), ": ", total_recv, " bytes read via socket ", fd, "\n");
			return total_recv;
		}
	}
}

int Monitor::_send_all(int i, const char * response, int size, struct socket & activeSocket)
{
	int fd = _pfds[i].fd;
	const char * chunk_send = response;
	int response_size = size, size_sent = 0, total_sent = 0;
	if (response_size < CHUNK_SIZE)	// cas où response initiale fait < 512
	{
		size_sent = send(fd, chunk_send, response_size, 0);
		// std::cout << size_sent << " bytes sent on socket " << fd << std::endl;
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	while (response_size > CHUNK_SIZE && size_sent != -1) // cas où response initiale > 512
	{
		size_sent = send(fd, chunk_send, CHUNK_SIZE, 0);
		// std::cout << size_sent << " bytes sent on socket " << fd << std::endl;
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	while (response_size > 0 && size_sent != -1) // envoie les derniers bytes lorsque response initiale était > 512 bytes ou lorsque send a pas fonctionné comme prévu
	{
		size_sent = send(fd, chunk_send, response_size, 0);
		// std::cout << size_sent << " bytes sent on socket " << fd << std::endl;
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	if (total_sent == size)
		log(get_time(), " Response to     ", activeSocket.client, " on server port ", activeSocket.server->get_port_str(),  ": ", total_sent, " bytes sent via socket ", fd, ", connection closed\n");
	else
		log(get_time(), " Response error: partial send to client " , activeSocket.client, " on server port ", activeSocket.server->get_port_str(),  ": ", total_sent, "/", size, "bytes sent via socket ", fd, ", connection closed\n");
	close(fd);
	_del_from_pfds(i);
	return total_sent;
}

// VOIR SI LA HEAP GROSSI PAS A L'INFINI SANS LIBERE DE L'ESPACE AVEC REALLOC ?
void Monitor::handle_connections()
{
	_prepare_master_sockets(); // socket, bind, listen pour chaque port + creer les struct pollfd dédiées
	int i, poll_index = 0;
	int poll_count = 0, server_count = _servers.size();
	std::string requestStr;
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";
	while (1)													// Main loop
	{
		poll_count = poll(_pfds, _fd_count, -1);				// bloque tant qu'aucun fd est prêt à read ou write
        if (poll_count < 0)
			log(get_time(), " Error: poll failed\n");
		i = poll_index;
		while (i < _fd_count)									// cherche parmi tous les fd ouverts
		{
			if (_pfds[i].revents & POLLIN)						// event sur fd[i]: si poll a debloqué pour un fd prêt à read
			{
				for (int j = 0; j < server_count; j++)
				{
					if (_pfds[i].fd == _master_sockets[j])		// si fd correspond a un socket de server en ecoute
					{
						_accept_new_connection(j);
						break;
					}
					if (j == server_count - 1)					// sinon fd correspond a un client qui fait une request
					{
						_recv_all(_pfds[i].fd, requestStr, _activeSockets[i]);
						try {
							Request request(requestStr.c_str());
							Response response(request, *(_activeSockets[i].server));

							// decomment to display in terminal:
							// std::cout << request.get_method() << " " << request.get_target() << " " << request.get_version() << std::endl;
							// request.display_fields();
							// std::cout << "\n" << request.get_body() << std::endl;

							// Response response(request);
						}
						catch (Request::MessageException & e) {
							std::cout << "Error: " << e.what() << std::endl; // A la fin, job de thomas de printer le message d'erreur comme c'est coherent (log ?)
						}
						requestStr.clear();
						_pfds[i].events = POLLOUT;
						poll_index = i;		// permet de revenir dans la main loop avec l'index du pfds à écrire
						i = _fd_count;		// break la while loop
					}
				}
			}
			else if (_pfds[i].revents & POLLOUT) 				// event sur fd[i]: si poll a debloquer pour un fd prêt à write
			{
				_send_all(i, response.c_str(), response.size(), _activeSockets[i]);
				poll_index = 0; // reset l'index au debut des fds
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
	{
		std::cerr << "Error while creating access log file" << std::endl;
		exit(1);
	}
}

void Monitor::_exit_cerr_msg(const std::string message, int code)
{
	log(get_time(), " Exit: ", message, "\n");
	std::cerr << "Exit: " << message;
	for (int i = 0; i < _fd_count; i++)
		close(_pfds[i].fd);
	if (_pfds)
		free(_pfds);
	if (_activeSockets)
		free(_activeSockets);
	_accessStream.close();
	exit(code);
}

std::string Monitor::get_time()
{
	time_t now = time(0);
    struct tm tstruct;
    char buf[80] = {0};
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
		_accessStream << "	" << "port: " << (*it).get_port_str() << std::endl;
		char ip4[INET_ADDRSTRLEN];
		_accessStream << "	" << "ip: " << (*it).get_ipv4_str() << std::endl;
		for (int j = 0; j < (*it).get_servernames().size(); j++)
			_accessStream << "	" << "server_name: " << (*it).get_servernames()[j] << std::endl;
		_accessStream << "	" << "root: " << (*it).get_root() << std::endl;
		std::list<std::string>::iterator it6 = (*it).get_indexes().begin();
		std::list<std::string>::iterator ite6 = (*it).get_indexes().end();
		for (; it6 != ite6; it6++)
			_accessStream << "	" << "index: " << (*it6) << std::endl;
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
			for (; it3 != (*it2).get_methods().end(); it3++)
				_accessStream << "		" << "method: " << (*it3) << std::endl;
			if ((*it2).get_autoindex() == true)
				_accessStream << "		" << "autoindex: " << "on" << std::endl;
			else
				_accessStream << "		" << "autoindex: " << "off" << std::endl;
			std::list<std::string>::iterator it4 = (*it2).get_indexes().begin();
			std::list<std::string>::iterator ite4 = (*it2).get_indexes().end();
			for (; it4 != ite4; it4++)
				_accessStream << "		" << "index: " << (*it4) << std::endl;
			_accessStream << "		" << "uploads_dir: " << (*it2).get_uploadsdir() << std::endl;
			if ((*it2).get_redirections().size())
			{
				std::list<Trio>::iterator it5 = (*it2).get_redirections().begin();
				std::list<Trio>::iterator ite5 = (*it2).get_redirections().end();
				for (; it5 != ite5; it5++)
					_accessStream << "		" << "redirect: " << (*it5).first << " " << (*it5).second << " " << (*it5).third << std::endl;
			}
			_accessStream << "		" << "cgi: " << (*it2).get_cgiBinPath() << std::endl;
		}
	}
	_accessStream << std::endl;
}
