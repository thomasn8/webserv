#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SERVERS_COUNT 2
#define PORT_1 80
#define PORT_2 8080
#define MAX_PENDING_CONNECTIONS 20
#define TRUE 1
#define FALSE 0

void exitWithError(std::string str, int code)
{
	perror(str.c_str()); 
	exit(code); 
}

int get_server_socket(int * master_socket, int * ports, int server_count, int max_connections)
{
	struct sockaddr_in addresses[server_count];
	int opt = TRUE;

	for (int i = 0; i < server_count; i++)
	{
		master_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
		fcntl(master_socket[i], F_SETFL, O_NONBLOCK);
		if (master_socket[i] < 0)
			std::cerr << "error: socket failed" << std::endl;
		if (setsockopt(master_socket[i], SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)) < 0)
			std::cerr << "error: setsockopt failed" << std::endl;
		
		memset(addresses[i].sin_zero, 0, sizeof(addresses[i].sin_zero));
		addresses[i].sin_family = AF_INET;
		addresses[i].sin_addr.s_addr = htonl(INADDR_ANY);
		addresses[i].sin_port = htons(ports[i]);
		addresses[i].sin_len = sizeof(addresses[i]);

		char ip4[INET_ADDRSTRLEN];
		std::cout << inet_ntop(AF_INET, &(addresses[i].sin_addr), ip4, INET_ADDRSTRLEN) << ":" << ntohs(addresses[i].sin_port);
		std::cout << " listening on socket fd: " << master_socket[i] << std::endl;
		
		if (bind(master_socket[i], (struct sockaddr *) &addresses[i], sizeof(addresses[i])) < 0)
			std::cerr << "error: bind failed" << std::endl;
		if (listen(master_socket[i], max_connections) < 0)
			return -1;
	}
	std::cout << std::endl;
	return 0;
}

int main()
{
	int master_socket[SERVERS_COUNT] = {0};
	int ports[SERVERS_COUNT] = {PORT_1, PORT_2};
	
	if (get_server_socket(master_socket, ports, SERVERS_COUNT, MAX_PENDING_CONNECTIONS) < 0)
		exitWithError("error getting listening socket\n", 1);

	fd_set all_fds;		// master file descriptor list
    fd_set ready_fds;	// temp file descriptor list for select()
    int fdmax;			// maximum file descriptor number

	struct sockaddr_in remoteaddr;
	remoteaddr.sin_len = sizeof(remoteaddr);
	int newfd;
	char buf[256] = {0};
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";

	// clear the socket set
	FD_ZERO(&all_fds);
	for (int i = 0; i < SERVERS_COUNT; i++)
	{
		// set the maximum fd number for select()
		if (master_socket[i] > fdmax)
			fdmax = master_socket[i];
		// pass file descriptors of each server in the select fd set
		FD_SET(master_socket[i], &all_fds);
	}
	FD_ZERO(&ready_fds);

	while (1)															// Main loop
	{
		ready_fds = all_fds;
        if (select(fdmax+1, &ready_fds, NULL, NULL, NULL) < 0)
			exitWithError("error: select failed\n", 1);
	// If timeout is not a null pointer, it specifies a maximum interval to wait for the selection to complete.
	// If timeout is a null pointer, the select blocks indefinitely.
	// To effect a poll, the timeout argument should be not be a null pointer, but it should point to a zero-valued timeval structure.

        for (int i = 0; i <= fdmax; i++)								// Run through the existing connections
		{
			if (FD_ISSET(i, &ready_fds))								// We have data to read in the existing connections
			{
				for (int j = 0; j < SERVERS_COUNT; j++)					// First, check only the master sockets and ...
				{
					if (i == master_socket[j])							// ... if the server is ready to read, accept() new connection
					{
						remoteaddr.sin_len = sizeof(remoteaddr);
						newfd = accept(master_socket[j], (struct sockaddr *)&remoteaddr, (socklen_t *)&remoteaddr.sin_len);
						if (newfd < 0)
							std::cerr << "error: accept failed\n";
						else
						{
							// fcntl(newfd, F_SETFL, O_NONBLOCK);
							FD_SET(newfd, &all_fds);
							if (newfd > fdmax)
								fdmax = newfd;
							std::cout << "Select-server: new connection on socket " << master_socket[j];
							std::cout << ", client socket fd is " << newfd;
							std::cout << ", client ip is " << inet_ntoa(remoteaddr.sin_addr);
							std::cout << ", client port is " <<  ntohs(remoteaddr.sin_port) << "\n\n";
						}
						break;
					}
					if (j == SERVERS_COUNT - 1)							// If not a master socket so a client is ready to write ...
					{
						int nbytes_recv = recv(i, buf, sizeof(buf), 0);	// ... handle his request (recv & send)
						
						if (nbytes_recv <= 0)							// Error or connection closed by client
						{
							if (nbytes_recv == 0)
								std::cout << "Select-server: connection closed on socket " << i << "\n";
							else
								std::cerr << "error: recv failed\n";
							close(i);
							FD_CLR(i, &all_fds);
						}
						else											// We got some good data from a client
						{
							std::cout << nbytes_recv << " bytes read on socket " << i << ":\n";
							std::cout << buf << std::endl;
							int nbytes_sent = send(i, response.c_str(), response.length(), 0);// ESSAYER D'ENVOYER 1 SEULE FOIS, DES QUE LA REQUETE ENTIERE RECVEID
							// int nbytes_sent = send(pfds[i].fd, buf, nbytes_recv, 0); // renvoyer le header
							if (nbytes_sent < 0)
								std::cerr << "error: send failed\n";
							else
								std::cout << "Welcome message sent successfully\n\n";
						}
					}
				}
			}
		}
	}
	return 0;
}

/*
	Règles:

	- It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the client and the server (listen included).
	- poll() (or equivalent) must check read and write at the same time.
	- You must never do a read or a write operation without going through poll() (or equivalent).
*/

/*
	// Clear an fd_set
	FD_ZERO(&readfds);  
	// Add a descriptor to an fd_set
	FD_SET(master_socket, &readfds);
	// Remove a descriptor from an fd_set
	FD_CLR(master_socket, &readfds); 
	// Is non-zero if fd is a member of fdset, zero otherwise
	FD_ISSET(master_socket, &readfds); 
	//  replaces an already allocated &fdset_copy file descriptor set with a copy of &fdset_orig
	FD_COPY(&fdset_orig, &fdset_copy);
	
	int activity;
	int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);
	check if some of their descriptors are ready for reading, are ready for writing, or have an exceptional condition pending, respectively.
	select() replaces the given descriptor sets with subsets consisting of those descriptors that are ready for the requested operation
	returns the total number of ready descriptors in all the sets or -1 (if the time limit expires, select() returns 0)
	If timeout is not a null pointer, it specifies a maximum interval to wait for the selection to complete otherwise the select blocks indefinitely.
	To effect a poll, the timeout argument should be not be a null pointer, but it should point to a zero-valued timeval structure.
	activity = select( MAX_FD + 1 , &readfds , NULL , NULL , NULL);
*/