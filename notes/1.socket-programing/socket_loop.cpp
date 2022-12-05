/*
	Un modèle de programme de socket pour apprendre à gérer du http,
	lire socket-programming.txt pour comprendre plus de détails

	compilable avec "make" ou "make loop"
	executable avec "make run"
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

int main()
{
	int server_fd, client_fd; 
	long client_read;
    struct sockaddr_in address;
	char buffer[30000] = {0};
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello from the server!";

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	address.sin_len = sizeof(address);
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, LISTEN_BACKLOG) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
		client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
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
