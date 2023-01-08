#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <poll.h>
#include <netinet/in.h>

#include "Server.hpp"

struct socket
{
	struct pollfd * pfd;
	struct sockaddr_in remoteAddr;
	Server * server;
	// keepalive timeouts ...
};

#endif
