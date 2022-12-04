#include <sys/socket.h>		// socket
#include <netinet/in.h>		// struct sockaddr_in
#include <string.h>			// memset
#include <stdio.h>			// perror

int main()
{
// #1
	// créer un point d'entrée pour la communication à venir
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("cannot create socket"); 
		return 1; 
	}

// #2
	// assigner une adresse locale au socket pour la connexion à venir
	const int PORT = 8080;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; 
	address.sin_port = htons(PORT);						// htons converts a short integer (e.g. port) to a network representation 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 		// htonl converts a long integer (e.g. address) to a network representation
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{ 
		perror("bind failed"); 
		return 1;
	}
	






	// if (listen(sfd, LISTEN_BACKLOG) == -1)
	// 	handle_error("listen");

	// /* Now we can accept incoming connections one
	// 	at a time using accept(2). */

	// peer_addr_size = sizeof(peer_addr);
	// cfd = accept(sfd, (struct sockaddr *) &peer_addr,
	// 			&peer_addr_size);
	// if (cfd == -1)
	// 	handle_error("accept");

	// /* Code to deal with incoming connection(s)... */

	// /* When no longer required, the socket pathname, MY_SOCK_PATH
	// 	should be deleted using unlink(2) or remove(3). */

	return 0;
}