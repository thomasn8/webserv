#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <poll.h>
#include <netinet/in.h>


struct socket
{
	struct pollfd * pfd;
	struct sockaddr_in remoteAddr;
	void * server;
	// timeout
};

#endif
