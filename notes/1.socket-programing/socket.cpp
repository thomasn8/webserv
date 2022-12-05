/*
	Un modèle de programme de socket avec commentaires pour apprendre à gérer du http,
	lire socket-programming.txt pour comprendre plus de détails

	c++ -Wall -Wextra -Werror -std=c++98  socket.cpp -o socket && ./socket
*/

#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>

#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080
#define LISTEN_BACKLOG 50
/* 
	defined in netinet/in.h:

	struct sockaddr_in 
	{ 
		__uint8_t         sin_len;
		sa_family_t       sin_family;
		in_port_t         sin_port;
		struct in_addr    sin_addr;
		char              sin_zero[8]; 
	};
*/

int main()
{
	int server_fd, client_fd; 
	long client_read;
    struct sockaddr_in address;
    std::string response;

// #1
	// crée un point d'entrée pour la communication à venir
	std::cout << "socket()" << std::endl;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}
	std::cout << "	server_fd: " << server_fd << std::endl;

// #2
	// attribution d'une struct pour contenir notamment l'adresse ip et le port
	// affiliation (bind) de cette struct au fd généré par socket() pour les connexions/communications à venir
	memset(address.sin_zero, 0, sizeof(address.sin_zero)); // = memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 	// htonl converts a long integer (address) to a network representation
	address.sin_port = htons(PORT);					// htons converts a short integer (port) to a network representation 
	address.sin_len = sizeof(address);
	std::cout << "bind()" << std::endl;
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE);
	}
	std::cout << "	address.sin_addr: " << address.sin_addr.s_addr << std::endl;
	std::cout << "	address.sin_port: " << address.sin_port << std::endl;

// #3
	// prépare notre socket à accepter des connexions
	std::cout << "listen()" << std::endl;
	if (listen(server_fd, LISTEN_BACKLOG) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	
	// accepte les connexions entrantes, une par une, et créer un nouveau socket (fd) pour le client
	// la struct address est complété avec l'adresse du client
	std::cout << "accept()" << std::endl;
	client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	if (client_fd < 0)
	{
		perror("accept failed");
		exit(EXIT_FAILURE);
	}
	// -> from here open localhost:8080 in webbrowser to connect to the socket
	std::cout << "	client_fd: " << client_fd << std::endl;
	std::cout << "	address.sin_addr: " << address.sin_addr.s_addr << std::endl;
	std::cout << "	address.sin_port: " << address.sin_port << std::endl;

// #4
	// read dans le fd du client pour voir les détails de sa requête
	char buffer[30000] = {0};
	client_read = read(client_fd, buffer, 30000); 
	if (client_read < 0)
		perror("No bytes are there to read");
	std::cout << std::endl << "client_read: " << std::endl << buffer;
	
	// write la réponse dans le fd du client
	response = "Hello from the server";							// !!! IMPORTANT !!!
	write(client_fd, response.c_str(), response.length());

// #5
	// close the client socket 
	close(client_fd);

// END
	return 0;
}
