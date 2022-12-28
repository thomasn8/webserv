#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 80
#define BACKLOG 10

void exitWithError(std::string str, int code)
{
	perror(str.c_str()); 
	exit(code); 
}

int main()
{
	int status, master_socket = 0, new_fd;
	struct addrinfo hints, *res, *p;
	char ipstr[INET6_ADDRSTRLEN];

	struct sockaddr_storage their_addr;
	socklen_t addr_size;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// get ready to connect
	// if ((status = getaddrinfo("www.mon-site.com", "http", &hints, &res)) != 0 )
	hints.ai_flags = AI_PASSIVE;  // tells getaddrinfo() to assign the address of my local host to the socket structures (NULL ptr in getaddrinfo)
	if ((status = getaddrinfo(NULL, "http", &hints, &res)) != 0)
	{
		gai_strerror(status);
		exitWithError("error: getaddrinfo failed", 1);
	}
	// res now points to a linked list of 1 or more struct addrinfo
	for(p = res; p != NULL; p = p->ai_next)
	{
		void *addr;

		// get the pointer to the address itself,
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;

		// convert the IP to a string and print it:
		addr = &(ipv4->sin_addr);
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("IP: %s:%u\n", ipstr, ntohs(ipv4->sin_port));
	}

	master_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (master_socket < 0)
		exitWithError("error: socket failed", 1);
	std::cout << "Socket: " << master_socket << std::endl;

	int yes = 1;	
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		exitWithError("error: multiple connections failed", 1);

	if (bind(master_socket, res->ai_addr, res->ai_addrlen) < 0)
		exitWithError("error: bind failed", 1);
	if (listen(master_socket, BACKLOG) < 0)
		exitWithError("error: listen failed", 1);

	addr_size = sizeof(their_addr);
	if (new_fd = accept(master_socket, (struct sockaddr *)&their_addr, &addr_size) < 0)
		exitWithError("error: accept failed", 1);


	freeaddrinfo(res); // free the linked-list

	return 0;
}
