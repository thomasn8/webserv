#include <sys/socket.h>		// socket
#include <netinet/in.h>		// struct sockaddr_in
#include <string.h>			// memset
#include <stdio.h>			// perror

int main()
{
// #1
	// crée un point d'entrée pour la communication à venir
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed"); 
		return 1; 
	}

// #2
	// attribution d'une struct pour contenir notamment l'adresse ip et le port
	// affiliation (bind) de cette struct au fd généré par socket() pour les connexions/communictions à venir
	const int PORT = 8080;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; 
	address.sin_port = htons(PORT);					// htons converts a short integer (port) to a network representation 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 	// htonl converts a long integer (address) to a network representation
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{ 
		perror("bind failed"); 
		return 1;
	}

// #3
	// prépare notre socket à accepter des connexions
	#define LISTEN_BACKLOG 50
	if (listen(server_fd, LISTEN_BACKLOG) < 0)
	{
		perror("listen failed");
		return 1;
	}
	// accept incoming connections one at a time and create a new socket (fd) for the client, stored in the struct
	int client_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	if (client_socket < 0)
	{
		perror("accept failed");
		return 1;    
	}

// #4
// #5


	return 0;
}	