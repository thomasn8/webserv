#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include <sys/socket.h>		// socket
#include <netinet/in.h>		// struct sockaddr_in
// struct sockaddr_in 
// { 
// 	__uint8_t         sin_len;
// 	sa_family_t       sin_family;
// 	in_port_t         sin_port;
// 	struct in_addr    sin_addr;
// 	char              sin_zero[8]; 
// };

int main()
{
// #1
	// crée un point d'entrée pour la communication à venir
	std::cout << "socket()" << std::endl;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed"); 
		return 1; 
	}
	std::cout << "	server_fd: " << server_fd << std::endl;

// #2
	// attribution d'une struct pour contenir notamment l'adresse ip et le port
	// affiliation (bind) de cette struct au fd généré par socket() pour les connexions/communications à venir
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 	// htonl converts a long integer (address) to a network representation
	const int PORT = 8080;
	address.sin_port = htons(PORT);					// htons converts a short integer (port) to a network representation 
	std::cout << "bind()" << std::endl;
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{ 
		perror("bind failed"); 
		return 1;
	}
	std::cout << "	address.sin_addr: " << address.sin_addr.s_addr << std::endl;
	std::cout << "	address.sin_port: " << address.sin_port << std::endl;

// #3
	// prépare notre socket à accepter des connexions
	#define LISTEN_BACKLOG 50
	std::cout << "listen()" << std::endl;
	if (listen(server_fd, LISTEN_BACKLOG) < 0)
	{
		perror("listen failed");
		return 1;
	}
	// accepte les connexions entrantes, une par une, et créer un nouveau socket (fd) pour le client
	// la struct address est complété avec l'adresse du client
	std::cout << "accept()" << std::endl;
	int client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	if (client_fd < 0)
	{
		perror("accept failed");
		return 1;
	}
	// -> from here open localhost:8080 in webbrowser to connect to the socket
	std::cout << "	client_fd: " << client_fd << std::endl;
	std::cout << "	address.sin_addr: " << address.sin_addr.s_addr << std::endl;
	std::cout << "	address.sin_port: " << address.sin_port << std::endl;

// #4
	// read dans le fd du client pour voir les détails de sa requête
	char buffer[1024] = {0};
	int client_read = read(client_fd, buffer, 1024); 
	if (client_read < 0)
		perror("No bytes are there to read");
	std::cout << std::endl << "client_read: " << std::endl << buffer;
	
	// write la réponse dans le fd du client
	std::string response = "Hello from the server";				// !!! IMPORTANT !!!
	write(client_fd, response.c_str(), response.length());

// #5
	// close the client socket 
	close(client_fd);

// END
	return 0;
}