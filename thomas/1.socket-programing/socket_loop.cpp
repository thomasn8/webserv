#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <unistd.h>
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
#define MAX_CLIENT_SOCKET 20
#define TRUE 1
#define FALSE 0

void exitWithError(std::string str, int code)
{
	perror(str.c_str()); 
	exit(code); 
}

int main()
{
	int master_socket[SERVERS_COUNT] = {0}, client_socket[MAX_CLIENT_SOCKET] = {0}, new_socket;
	int ports[SERVERS_COUNT] = {PORT_1, PORT_2};
	int opt = TRUE;
    struct sockaddr_in addresses[SERVERS_COUNT];

	for (int i = 0; i < SERVERS_COUNT; i++)
	{
		// crée un point d'entrée pour la communication à venir
		// paramètre le socket pour permettre plusieurs connexion simultannée,
		master_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (master_socket[i] < 0)
			exitWithError("error: master socket failed", 1);
		if (setsockopt(master_socket[i], SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)) < 0)
			exitWithError("error: multiple connections failed", 1);
		
		// attribution d'une struct pour contenir notamment l'adresse ip et le port
		memset(addresses[i].sin_zero, 0, sizeof(addresses[i].sin_zero));
		addresses[i].sin_family = AF_INET;

		// convert string IP address to its binary representations
		addresses[i].sin_addr.s_addr = htonl(INADDR_ANY);
		// is like: inet_pton(AF_INET, "0.0.0.0", &(addresses[i].sin_addr)) OR like: inet_addr("0.0.0.0");
		// reverse is: inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

		// convert string port number to its binary representations
		addresses[i].sin_port = htons(ports[i]);
		addresses[i].sin_len = sizeof(addresses[i]);

		char ip4[INET_ADDRSTRLEN];
		std::cout << inet_ntop(AF_INET, &(addresses[i].sin_addr), ip4, INET_ADDRSTRLEN) << ":" << ntohs(addresses[i].sin_port);
		// std::cout << ntohl(addresses[i].sin_addr.s_addr) << ":" << ntohs(addresses[i].sin_port);
		std::cout << " listening on socket fd: " << master_socket[i] << std::endl;
		
		// affiliation (bind) de cette struct au fd généré par socket() pour les connexions/communications à venir
		if (bind(master_socket[i], (struct sockaddr *) &addresses[i], sizeof(addresses[i])) < 0)
			exitWithError("error: bind failed", 1);
		// prépare notre socket à accepter des connexions
		if (listen(master_socket[i], MAX_PENDING_CONNECTIONS) < 0)
			exitWithError("error: listen failed", 1);
	}
	std::cout << std::endl;

	// creer un set de filedescriptors pour gerer des connexions sur plusieurs servers/ports
	fd_set readfds;
	int max_fd = -1;
	char buffer[30000] = {0};
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!\n";
	while(1)
	{
		// clear the socket set
		FD_ZERO(&readfds);
		for (int i = 0; i < SERVERS_COUNT; i++)
		{
			// set the maximum fd number for select()
			if (master_socket[i] > max_fd)
				max_fd = master_socket[i];
			// pass file descriptors of each server in the select fd set
			FD_SET(master_socket[i], &readfds);
		}
		// Once you have an incoming connection, the select would unblock and replaces the given descriptor sets with subsets consisting of those descriptors that are ready for the requested operation
		if (select(max_fd + 1 , &readfds , NULL , NULL , NULL) < 0)
			std::cerr << "error: select failed" << std::endl;

		for (int i = 0; i < SERVERS_COUNT; i++)
		{
			// If something happened on the master socket, then its an incoming connection
			if (FD_ISSET(master_socket[i], &readfds))
			{
				// Then accept will extracts the first connection request on the queue of pending connections, creates a new socket with the same properties of socket, and allocates a new file descriptor for the socket
				// The argument address is a result parameter that is filled in with the address of the connecting entity, as known to the communications layer.
				if ((new_socket = accept(master_socket[i], (struct sockaddr *)&addresses[i], (socklen_t *)&addresses[i].sin_len)) < 0)
					std::cerr << "error: accept failed" << std::endl;
				else
				{
					std::cout << "New connection on socket " << master_socket[i] << ": client socket fd is " << new_socket << ", client ip is " << inet_ntoa(addresses[i].sin_addr) << ", client port is " <<  ntohs(addresses[i].sin_port) << std::endl;
					// int client_read = read(new_socket, buffer, 30000);
					// if (client_read < 0)
					// 	std::cerr << "error: read: no bytes received from client" << std::endl;
					// // std::cout << buffer << std::endl;
					// write(new_socket, response.c_str(), response.length());
					
					/* OR USE OF send()/recv() */

					int client_read = recv(new_socket, buffer, 30000, 0);
					if (client_read < 0)
						std::cerr << "error: read: no bytes received from client" << std::endl;
					std::cout << buffer << std::endl;
					int byte_sent = send(new_socket, response.c_str(), response.length(), 0);
		
					std::cout << client_read << " bytes read. Welcome message sent successfully\n";
					// if (client_read == 0)
					// {
						std::cout << "connection terminated. close socket " << new_socket << std::endl << std::endl;
						close(new_socket);
					// }
				}
			}
		}
	}
	return 0;
}

/*
	// Clear an fd_set
	FD_ZERO(&readfds);  
	// Add a descriptor to an fd_set
	FD_SET(master_socket, &readfds);
	// Remove a descriptor from an fd_set
	FD_CLR(master_socket, &readfds); 
	// is non-zero if fd is a member of fdset, zero otherwise
	//If something happened on the master socket , then its an incoming connection  
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