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
	// CLOSE ET FREE TOUT CE QUI FAUT DANS LA PARTIE SOCKET
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
void Monitor::_prepare_sockets()
{
	it_servers it = _servers.begin();
	it_servers ite = _servers.end();
	int socket_fd;
	while (it != ite)
	{
		socket_fd = (*it).create_socket();
		_master_sockets.push_back(socket_fd);
		_fd_count++;
		it++;
	}
	_fd_capacity = _fd_count + 5;
	_pfds = (struct pollfd *)malloc(sizeof(*_pfds) * _fd_capacity);
	for (int i = 0; i < _fd_count; i++)
	{
		_pfds[i].fd = _master_sockets[i];
    	_pfds[i].events = POLLIN;
    	// _pfds[i].events = POLLIN | POLLOUT;
    	_pfds[i].revents = 0;
	}
}

void Monitor::_add_to_pfds(int newfd)
{
	if (_fd_count == _fd_capacity)
	{
		_fd_capacity *= 2;
		_pfds = (struct pollfd *)realloc(_pfds, sizeof(*_pfds) * (_fd_capacity));
	}
	_pfds[_fd_count].fd = newfd;
	_pfds[_fd_count].events = POLLIN;
	// _pfds[_fd_count].events = POLLIN | POLLOUT;
	_pfds[_fd_count].revents = 0 ;
	_fd_count++;
}

void Monitor::_del_from_pfds(int i)
{
	_pfds[i] = _pfds[_fd_count - 1];
	_fd_count--;
}

int Monitor::_recv_chunks(int s)
{
	int size_recv , total_size = 0;
	char chunk[CHUNK_SIZE];
	while(1)
	{
		memset(chunk, 0, CHUNK_SIZE);
		if((size_recv =  recv(s, chunk, CHUNK_SIZE, 0) ) < 0)
			break;
		else
		{
			total_size += size_recv;
			printf("%s" , chunk);
		}
	}
	return total_size;
}

void Monitor::handle_connections()
{
	_prepare_sockets();

	int server_count = _servers.size();
	std::string response = "HTTP/1.1 200 OK\nContent-Length: 0\n\n";
	std::string finalResponse = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";
	int poll_count = 0;
	int newfd = -1;
	int size_recv = 0, total_recv = 0, size_sent = 0;
	char chunk[CHUNK_SIZE];
	// char buf[500];
	struct sockaddr_in remoteAddr;
	while (1)													// Main loop
	{
		poll_count = poll(_pfds, _fd_count, -1);
        if (poll_count < 0)
			_exit_cerr_msg("Error: poll failed\n", 1);
			// log("Error: poll failed\n");
		for (int i = 0; i < _fd_count; i++)						// Run through the existing connections
		{
			// if (_pfds[i].revents & POLLIN)						// We have data to read in the existing connections
			// if (_pfds[i].revents & POLLIN | _pfds[i].revents & POLLOUT)						// We have data to read in the existing connections
			// if (_pfds[i].revents == POLLIN + POLLOUT)						// We have data to read in the existing connections
			// if (_pfds[i].revents == POLLIN)						// We have data to read in the existing connections
			// if (_pfds[i].revents == READ_AND_WRITE)						// We have data to read in the existing connections
			// if ((_pfds[i].revents & POLLIN) == POLLIN && (_pfds[i].revents & POLLOUT) == POLLOUT)						// We have data to read in the existing connections
			if (_pfds[i].revents != 0) 
			{
				printf("  fd=%d; events: %s%s%s%s\n", _pfds[i].fd,
						(_pfds[i].revents & POLLIN)  ? "POLLIN "  : "",
						(_pfds[i].revents & POLLOUT)  ? "POLLOUT "  : "",
						(_pfds[i].revents & POLLHUP) ? "POLLHUP " : "",
						(_pfds[i].revents & POLLERR) ? "POLLERR " : "");
			}
			if (_pfds[i].revents & POLLIN)
			{
				std::cerr << "FD READY TO READ\n";
				for (int j = 0; j < server_count; j++)			// First, check only the master sockets and ...
				{
					if (_pfds[i].fd == _master_sockets[j])			// ... if the server is ready to read, accept() new connection
					{
						remoteAddr.sin_len = sizeof(remoteAddr);
						newfd = accept(_master_sockets[j], (struct sockaddr *)&remoteAddr, (socklen_t *)&remoteAddr.sin_len);
						if (newfd < 0)
							std::cerr << "error: accept failed\n";
						else
						{
							_add_to_pfds(newfd);
							std::cout << "Poll-server: new connection on socket " << _master_sockets[j];
							std::cout << ", client socket fd is " << newfd;
							std::cout << ", client ip is " << inet_ntoa(remoteAddr.sin_addr);
							// std::cout << ", client port is " <<  ntohs(remoteAddr.sin_port) << "\n\n";
							
							std::cout << ", client port is " <<  ntohs(remoteAddr.sin_port);
							// int opt = 1;
							// setsockopt(newfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&opt, sizeof(opt));
							// struct timeval tv;
							// tv.tv_sec = 10;
							// tv.tv_usec = 0;
							// setsockopt(newfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
							int rcvBufferSize1;
							int sockOptSize1 = sizeof(rcvBufferSize1);
							getsockopt(newfd, SOL_SOCKET, SO_KEEPALIVE, &rcvBufferSize1, (socklen_t *)&sockOptSize1);
							int rcvBufferSize2;
							int sockOptSize2 = sizeof(rcvBufferSize2);
							getsockopt(newfd, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize2, (socklen_t *)&sockOptSize2);
							int rcvBufferSize3;
							int sockOptSize3 = sizeof(rcvBufferSize3);
							getsockopt(newfd, SOL_SOCKET, SO_RCVTIMEO, &rcvBufferSize3, (socklen_t *)&sockOptSize3);
							std::cout << "SO_KEEPALIVE: " << rcvBufferSize1 << ", SO_RCVBUF: " << rcvBufferSize2 << ",  SO_RCVTIMEO: " << rcvBufferSize3 << "\n\n";
						}
						break;
					}
					if (j == server_count - 1)					// If not a master socket so a client is ready to write ...
					{
						while(1)
						{
							memset(chunk, 0, CHUNK_SIZE);
							size_recv = recv(_pfds[i].fd, chunk, CHUNK_SIZE, 0);	// ... handle his request (recv & send)
							if (size_recv <= 0)
							{
								// if (size_recv < 0)
								// {
								// 	std::cerr << "error: recv failed\n";
								// 	// printf("Error (%d) %s\n", errno, strerror(errno));
								// }
								close(_pfds[i].fd);
								_del_from_pfds(i);
								std::cout << "Poll-server: connection closed on socket " << _pfds[i].fd << "\n";
								break;
							}
							else
							{
								total_recv += size_recv;
								std::cout << std::endl << size_recv << " bytes read on socket " << _pfds[i].fd << ":\n";
								std::cout << chunk;
								size_sent = send(_pfds[i].fd, finalResponse.c_str(), finalResponse.length(), 0);
								if (size_sent < 0)
									std::cerr << "error: send failed\n";
							}
							std::cout << std::endl;
						}

						// memset(chunk, '\0', CHUNK_SIZE);
						// size_recv = recv(_pfds[i].fd, chunk, CHUNK_SIZE, 0);	// ... handle his request (recv & send)
						// // try {
						// // 	Request request(buf);
						// // 	Response response(request);
						// // }
						// // catch (MessageException const & e) {
						// // 	Response response(e.what);
						// // }
						// if (size_recv <= 0)					// Error or connection closed by client
						// {
						// 	if (size_recv < 0)
						// 		std::cerr << "error: recv failed\n";
						// 	close(_pfds[i].fd);
						// 	_del_from_pfds(i);
						// 	std::cout << "Poll-server: connection closed on socket " << _pfds[i].fd << "\n";
						// 	// std::cout << "char[0] = |" << chunk[size_recv] << "|\n\n";
						// }
						// else									// We got some good data from a client
						// {
						// 	std::cout << size_recv << " bytes read on socket " << _pfds[i].fd << ":\n";
						// 	std::cout << chunk << std::endl;
						// 	size_sent = send(_pfds[i].fd, finalResponse.c_str(), finalResponse.length(), 0);
						// 	if (size_sent < 0)
						// 		std::cerr << "error: send failed\n";
						// 	// std::cout << "char[477] = |" << chunk[size_recv - 2] << "|\n\n";
						// 	// std::cout << "char[478] = |" << chunk[size_recv - 1] << "|\n\n";	// dernier char du buffer
						// }
					}
				}
			}
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
		_accessStream << "	" << "ip: " << ntohl((*it).get_address()) << std::endl;
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
