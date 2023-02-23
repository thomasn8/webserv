#include "../includes/Monitor.hpp"
/* 
	************ CONST/DESTR
*/
Monitor::Monitor(char **env) :
_env(env),
_servers(std::deque<Server>()),
_master_sockets(NULL),
_master_size(0),
_fd_count(0),
_fd_capacity(0),
_pfds(NULL),
_activeSockets(NULL),
_logFile(std::string(LOG_PATH)),
_log()
{
	_create_log_file(_logFile, _log);
}

Monitor::~Monitor()
{
	for (int i = 0; i < _fd_count; i++)
		close(_pfds[i].fd);
	if (_pfds)
		free(_pfds);
	if (_activeSockets)
		free(_activeSockets);
	if (_master_size)
		free(_master_sockets);
	if (_buf.capacity > 0)
		free(_buf.begin);
	_log << get_time() << " Server shut down" << std::endl;
	std::cout << "Server shut down" << std::endl;
	_log.close();
}

/* 
	************ GETTERS/SETTERS
*/
std::deque<Server> & Monitor::get_servers() { return _servers; }

void Monitor::add_server() { _servers.push_back(Server()); }

/* 
	************ SOCKETS
*/
void Monitor::_prepare_master_sockets()
{
	_log << get_time() << " SERVER STARTED" << std::endl;
	it_servers it = _servers.begin();
	it_servers ite = _servers.end();
	int socket_fd;
	int i = 0;
	_master_size = _servers.size();
	_master_sockets = (int *)malloc(_master_size * sizeof(int *));
	while (it != ite)
	{
		socket_fd = (*it).create_socket(_env);
		_log << "port " << (*it).get_port_str() << " listening on socket " << socket_fd << std::endl;
		_master_sockets[i] = socket_fd;
		_fd_count++;
		it++;
		i++;
	}
	_fd_capacity = _fd_count + 5;
	_pfds = (struct pollfd *)malloc(sizeof(struct pollfd) * _fd_capacity);
	_activeSockets = (struct socket *)malloc(sizeof(struct socket) * _fd_capacity);
	if (_pfds == NULL)
	{
		_log << "Error: impossible to allocate" << _fd_capacity << " pollfd structs" << std::endl;
		_exit_cerr_msg("Fatal error: allocation\n", 1);
	}
	for (int i = 0; i < _fd_count; i++)
	{
		_pfds[i].fd = _master_sockets[i];
    	_pfds[i].events = POLLIN;
    	_pfds[i].revents = 0;
		_activeSockets[i].pfd = NULL;
	}
	_log << std::endl;
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
			_log << "Error: impossible to allocate" << _fd_capacity << " pollfd structs" << std::endl;
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
	std::string ip = inet_ntoa(activeSocket->remoteAddr.sin_addr);
	std::string port = std::to_string(ntohs(activeSocket->remoteAddr.sin_port)).c_str();
	while (ip.c_str()[i])
	{
		activeSocket->client[i]  = ip.c_str()[i];
		i++;
	}
	activeSocket->client[i++] = ':';
	int j = 0;
	while (port.c_str()[j])
	{
		activeSocket->client[i] = port.c_str()[j];
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
	socklen_t remoteAddr_size = sizeof(remoteAddr);
	int new_fd = accept(_master_sockets[master_index], (struct sockaddr *)&remoteAddr, &remoteAddr_size);
	if (new_fd < 0)
		_log << get_time() << " Error: accept: new connexion on port " << _servers[master_index].get_port_str() << " failed" << std::endl;
	else
		struct socket * client_socket = _add_to_pfds(new_fd, &remoteAddr, &_servers[master_index]);
}

int Monitor::_replace_alone_header_cr() 
{
	char *ptr = _buf.begin;
	size_t len = _buf.size, i = 0;
	while (i < len)
	{
		if (ptr[i] == '\r')
		{
			if ((i+1 < len && ptr[i+1] != '\n') || (i+1 == len))
				ptr[i] = ' ';
			else if (i+3 < len && ptr[i+1] == '\n' && ptr[i+2] == '\r' && ptr[i+3] == '\n')
				return 0;
		}
		i++;
	}
	if (i > MHS)
		return -1;
	return 0;
}

ssize_t Monitor::_recv_all(int fd, struct socket & activeSocket)
{
	ssize_t size_recv = 0, maxrecv = activeSocket.server->get_maxrecv();
	_buf.size = 0;
	_buf.current = _buf.begin;
	_recv_timeout[0] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while (1)
	{
		_recv_timeout[1] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (_recv_timeout[1] - _recv_timeout[0] > RECV_TIMEOUT_MS)
			return -1;	// A VOIR SI ON VEUT PAS RETOURNER UNE AUTRE VALEUR POUR ADAPTER LE CODE D'ERREUR RETRANSMIS DANS LE MONITOR ggbgggg
		if (_buf.size + CHUNK_RECV > _buf.capacity)
		{
			if (_buf.capacity == 0)
			{
				_buf.begin = (char *)malloc(CHUNK_RECV);
				_buf.capacity = CHUNK_RECV;
			}
			else
			{
				_buf.begin = (char *)realloc(_buf.begin, _buf.capacity * 2);
				_buf.capacity *= 2;
			}
			if (_buf.begin == NULL)
				return -1;
			_buf.current = _buf.begin + _buf.size;
		}
		size_recv = recv(fd, _buf.current, CHUNK_RECV, 0); // recv la request jusqu'au bout du client_fd
		_buf.size += size_recv;
		_buf.current += size_recv;
		// if (maxrecv && _buf.size > maxrecv) // erreur max body size 413
		// 	return -1;
		if (size_recv < CHUNK_RECV) // toute la request a été read
		{
			_log << get_time() << " Request from    " << activeSocket.client << " on server port " << activeSocket.server->get_port_str() << ": socket " << fd << ",	read " << _buf.size << " bytes" << std::endl;
			if (maxrecv && _buf.size > maxrecv) // erreur max body size 413
				return -1;
			return _buf.size;
		}
	}
}

int Monitor::_send_all(int i, const char * response, int size, struct socket & activeSocket)
{
	int fd = _pfds[i].fd;
	const char * chunk_send = response;
	ssize_t response_size = size, size_sent = 0, total_sent = 0;
	_sent_timeout[0] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (response_size < CHUNK_SEND)	// cas où response initiale fait < CHUNK_SEND
	{
		size_sent = send(fd, chunk_send, response_size, 0);
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	while (response_size > CHUNK_SEND && size_sent != -1) // cas où response initiale > CHUNK_SEND
	{
		_sent_timeout[1] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (_sent_timeout[1] - _sent_timeout[0] > SEND_TIMEOUT_MS)
			break;
		size_sent = send(fd, chunk_send, CHUNK_SEND, 0);
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	while (response_size > 0 && size_sent != -1) // envoie les derniers bytes lorsque response initiale était > CHUNK_SEND bytes ou lorsque send a pas fonctionné comme prévu
	{
		_sent_timeout[1] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (_sent_timeout[1] - _sent_timeout[0] > SEND_TIMEOUT_MS)
			break;
		size_sent = send(fd, chunk_send, response_size, 0);
		response_size -= size_sent;
		chunk_send += size_sent;
		total_sent += size_sent;
	}
	if (total_sent == size)
		_log << get_time() << " Response to     " << activeSocket.client << " on server port " << activeSocket.server->get_port_str() << ": socket " << fd << ",	send " << total_sent << " bytes" << std::endl;
	else
		_log << get_time() << " Response error: partial send to client " << activeSocket.client << " on server port " << activeSocket.server->get_port_str() << ": socket " << fd << ",	send " << total_sent << "/" << size << "bytes" << std::endl;
	return total_sent;
}

// si on était pas en NON-BLOCKING mode, accept() bloquerait le server et le server pourrait gérer qu'1 seule connection simultanée.
// Pareil pour le read() de recv() (comme quand on veut écrire dans un pipe et que l'autre process est bloqué par le read tant que rien est write dans le pipe)
void Monitor::handle_connections()
{
	_prepare_master_sockets();
	int i, poll_index = 0, poll_count = 0, server_count = _servers.size();
	_buf.capacity = 0;
	struct responseInfos res;
	while (1)																						// Main loop
	{
		poll_count = poll(_pfds, _fd_count, POLL_TIMEOUT);											// bloque tant qu'aucun fd est prêt à read ou write
        if (poll_count < 0)
			_log << get_time() << " Error: poll failed" << std::endl;
		i = poll_index;
		while (i < _fd_count)																		// cherche parmi tous les fd ouverts
		{
			if (_pfds[i].revents & POLLIN)															// event sur fd[i] si poll a debloqué pour un fd prêt à read
			{
				for (int j = 0; j < server_count; j++)
				{
					if (_pfds[i].fd == _master_sockets[j])											// si fd correspond a un socket de server en ecoute
					{
						_accept_new_connection(j);
						break;
					}
					if (j == server_count - 1)														// sinon fd correspond a un client qui fait une request
					{
						_start_chrono();
						if (_recv_all(_pfds[i].fd, _activeSockets[i]) != -1)
						{
							if (_replace_alone_header_cr() != -1)
							{
								std::string requestStr(_buf.begin, _buf.size);
								try {
									Request request(&requestStr, _activeSockets[i].server);					// essaie de constr une requeste depuis les donnees recues
									Response response(&request, _activeSockets[i].server, &res);			// essaie de constr une response si on a une request
								}
								catch (StatusCodeException & e) {
									Response response(e.statuscode(), _activeSockets[i].server, &res);		// si request a un probleme, construit une response selon son status code
								}
							}
							else
								Response response(HEADERS_TOO_LARGE, _activeSockets[i].server, &res);
						}
						else
							Response response(PAYLOAD_TOO_LARGE, _activeSockets[i].server, &res);			// si recvall a atteint le MBS, constuit une response selon le status code
						if (_buf.capacity > BUFFER_LIMIT)
						{
							free(_buf.begin);
							_buf.capacity = 0;
						}
						_pfds[i].events = POLLOUT;
						poll_index = i; // permet de revenir dans la main loop avec l'index du pfds à write
						i = _fd_count;  // break la while loop
					}
				}
			}
			else if (_pfds[i].revents & POLLOUT || _pfds[i].revents & POLLHUP) 						// event sur fd[i], i poll a debloquer pour un fd prêt à write ou connection perdue sur le socket en question
			{
				if (_pfds[i].revents & POLLOUT)
				{
					_send_all(i, res.header.c_str(), res.header.size(), _activeSockets[i]); 		// send le header
					if (res.body_size)
						_send_all(i, res.body, res.body_size, _activeSockets[i]); 					// send le body
				}
				_stop_chrono(_pfds[i].fd);
				close(_pfds[i].fd);
				_del_from_pfds(i);
				free(res.body);
				poll_index = 0;
			}
			i++;
		}
	}
}

void Monitor::_start_chrono()
{
	_chrono_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Monitor::_stop_chrono(int fd)
{
	_chrono_stop = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	_chrono_stop -= _chrono_start;
	_log << "Socket " << fd << ": connection closed, communication time = " << _chrono_stop << " ms" << std::endl;
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
	_log << get_time() << " Exit: " << message << std::endl;
	std::cerr << "Exit: " << message;
	for (int i = 0; i < _fd_count; i++)
		close(_pfds[i].fd);
	if (_pfds)
		free(_pfds);
	if (_activeSockets)
		free(_activeSockets);
	if (_master_size)
		free(_master_sockets);
	if (_buf.capacity > 0)
		free(_buf.begin);
	_log.close();
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
