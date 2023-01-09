#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <poll.h>
#include <netinet/in.h>

#include "Server.hpp"

struct socket
{
	struct pollfd * pfd;
	struct sockaddr_in remoteAddr;
	char client[21]; // peut contenir 255.255.255.255:65535
	Server * server;
	// keepalive timeouts ...
};

#endif
