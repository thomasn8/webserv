/*
	Un modèle de programme de socket pour apprendre à gérer du http,
	lire socket-programming.txt pour comprendre plus de détails

	compilable avec "make" ou "make loop"
	executable avec "make run"
*/

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERS_COUNT 2
#define PORT_1 80
#define PORT_2 8080
#define MAX_PENDING_CONNECTIONS 30
#define MAX_CLIENT_SOCKET 30
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
		addresses[i].sin_addr.s_addr = htonl(INADDR_ANY);
		addresses[i].sin_port = htons(ports[i]);
		addresses[i].sin_len = sizeof(addresses[i]);

		std::cout << ntohl(addresses[i].sin_addr.s_addr) << ":" << ntohs(addresses[i].sin_port);
		std::cout << " listening on socket fd: " << master_socket[i] << std::endl;
		
		// affiliation (bind) de cette struct au fd généré par socket() pour les connexions/communications à venir
		if (bind(master_socket[i], (struct sockaddr *) &addresses[i], sizeof(addresses[i])) < 0)
			exitWithError("error: bind failed", 1);
		// prépare notre socket à accepter des connexions
		if (listen(master_socket[i], MAX_PENDING_CONNECTIONS) < 0)
			exitWithError("error: listen failed", 1);
	}

	// creer un set de filedescriptors pour gerer des connexions sur plusieurs servers/ports
	fd_set readfds;
	int max_fd = -1;
	long client_read;
	char buffer[30000] = {0};
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!";
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
					std::cout << "New connection, socket fd is " << new_socket << ", client ip is :" << inet_ntoa(addresses[i].sin_addr) << ", client port is :" <<  ntohs(addresses[i].sin_port) << std::endl;
					client_read = read(new_socket, buffer, 30000);
					if (client_read < 0)
						std::cerr << "error: read: no bytes received from client" << std::endl;
					std::cout << buffer << std::endl;
					write(new_socket, response.c_str(), response.length());
					std::cout << "Welcome message sent successfully" << std::endl;
					close(new_socket);
				}
			}
		}
	}
	return 0;
}
