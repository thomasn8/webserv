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
		{
			std::cerr << "error: bind failed" << std::endl;
		(master_socket[i]);
		}
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

	struct sockaddr_in remoteaddr;
	remoteaddr.sin_len = sizeof(remoteaddr);
	int newfd;
	char buf[256] = {0};
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";


	while(1)
	{
		for (int i = 0; i < SERVERS_COUNT; i++)
		{
			remoteaddr.sin_len = sizeof(remoteaddr);
			newfd = accept(master_socket[i], (struct sockaddr *) &remoteaddr, (socklen_t *) &remoteaddr.sin_len);
			// If no pending connections are present on the queue, and the socket is not marked as non-blocking, accept() blocks the caller until a connection is present.
			// If the socket is marked non-blocking and no pending connections are present on the queue, accept() returns an error as described below.
			// The accepted socket may not be used to accept more connections. The original socket socket, remains open.
			// The call returns -1 on error and the global variable errno is set to indicate the error. If it succeeds, it returns a non-negative integer that is a descriptor for the accepted socket.
			// The accept() system call will fail if:
			// [EWOULDBLOCK]      socket is marked as non-blocking and no connections are present to be accepted.
			if (newfd < 0)
				std::cerr << "no pending connection" << std::endl;
				// std::cerr << "error: accept failed" << std::endl;
			else
			{

				std::cout << "Server: new connection on socket " << master_socket[i];
				std::cout << ", client socket fd is " << newfd;
				std::cout << ", client ip is " << inet_ntoa(remoteaddr.sin_addr);
				std::cout << ", client port is " <<  ntohs(remoteaddr.sin_port) << "\n\n";

				std::cout << "New connection on socket " << master_socket[i] << ": client socket fd is " << new_socket << ", client ip is " << inet_ntoa(addresses[i].sin_addr) << ", client port is " <<  ntohs(addresses[i].sin_port) << std::endl;
				// int client_read = read(new_socket, buffer, 30000);
				// if (client_read < 0)
				// 	std::cerr << "error: read: no bytes received from client" << std::endl;
				// // std::cout << buffer << std::endl;
				// write(new_socket, response.c_str(), response.length());
				
				/* OR USE OF send()/recv() */

				int client_read = recv(new_socket, buf, 256, 0);
				if (client_read < 0)
					std::cerr << "error: read: no bytes received from client" << std::endl;
				std::cout << buf << std::endl;
				int byte_sent = send(new_socket, response.c_str(), response.length(), 0);
	
				std::cout << client_read << " bytes read. Welcome message sent successfully\n";
				// if (client_read == 0)
				// {
					std::cout << "connection terminated. close socket " << new_socket << std::endl << std::endl;
					close(new_socket);
				// }


				// std::cout << "New connection, socket fd is " << new_socket << ", client ip is :" << inet_ntoa(addresses[i].sin_addr) << ", client port is :" <<  ntohs(addresses[i].sin_port) << std::endl;
				// client_read = read(new_socket, buffer, 30000);
				// if (client_read < 0)
				// 	std::cerr << "error: read: no bytes received from client" << std::endl;
				// std::cout << buffer << std::endl;
				// write(new_socket, response.c_str(), response.length());
				// std::cout << "Welcome message sent successfully" << std::endl;
				// close(new_socket);
			}
		}


		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
		newfd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);

		if (client_fd < 0)
		{
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		client_read = read(client_fd, buffer, 30000);
		if (client_read < 0)
			perror("No bytes are there to read");
		std::cout << buffer << std::endl;
		write(client_fd, response.c_str(), response.length());
		std::cout << "------- Hello message sent -------" << std::endl;
		close(client_fd);
	}
	return 0;
}