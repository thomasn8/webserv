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
#include <poll.h>

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

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = (struct pollfd *) realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }
    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read
    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
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
	
    int fd_count = 0;
    int fd_size = 5 + SERVERS_COUNT;
    struct pollfd *pfds = (struct pollfd *)malloc(sizeof(*pfds) * fd_size);
	if (get_server_socket(master_socket, ports, SERVERS_COUNT, MAX_PENDING_CONNECTIONS) < 0)
		exitWithError("error getting listening socket\n", 1);
	for (int i = 0; i < SERVERS_COUNT; i++)
	{
		pfds[i].fd = master_socket[i];
    	pfds[i].events = POLLIN; 		// Report ready to read on incoming connection
    	// pfds[i].events = POLLOUT; 		// Report ready to read on incoming connection
	}
    fd_count = SERVERS_COUNT;

	struct sockaddr_in remoteaddr;
	int newfd;
	char buf[256] = {0};
	// char buf[1024] = {0};
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nHello from the server!\n";

	while (1)													// Main loop
	{
		int poll_count = poll(pfds, fd_count, -1);
		// int poll_count = poll(pfds, fd_count, 0);
// If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait for any file descriptor to become ready.
// If timeout is zero, then poll() will return without blocking.
// If the value of timeout is -1, the poll blocks indefinitely.
        if (poll_count < 0)
			exitWithError("error: poll failed\n", 1);
		// if (poll_count > 0)
		// {
			// std::cerr << "POLLING\n";
			for (int i = 0; i < fd_count; i++)						// Run through the existing connections
			{
				if (pfds[i].revents & POLLIN)						// We have data to read in the existing connections
				{
					std::cerr << "FD READY TO READ\n";
					for (int j = 0; j < SERVERS_COUNT; j++)			// First, check only the master sockets and ...
					{
						if (pfds[i].fd == master_socket[j])			// ... if the server is ready to read, accept() new connection
						{
							remoteaddr.sin_len = sizeof(remoteaddr);
							newfd = accept(master_socket[j], (struct sockaddr *)&remoteaddr, (socklen_t *)&remoteaddr.sin_len);
							if (newfd < 0)
								std::cerr << "error: accept failed\n";
							else
							{
								// fcntl(newfd, F_SETFL, O_NONBLOCK);
								add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
								std::cout << "Poll-server: new connection on socket " << master_socket[j];
								std::cout << ", client socket fd is " << newfd;
								std::cout << ", client ip is " << inet_ntoa(remoteaddr.sin_addr);
								std::cout << ", client port is " <<  ntohs(remoteaddr.sin_port) << "\n\n";
							}
							break;
						}
						if (j == SERVERS_COUNT - 1)					// If not a master socket so a client is ready to write ...
						{
							int nbytes_recv = recv(pfds[i].fd, buf, sizeof(buf), 0);	// ... handle his request (recv & send)
							int sender_fd = pfds[i].fd;
							
							if (nbytes_recv <= 0)					// Error or connection closed by client
							{
								if (nbytes_recv == 0)
									std::cout << "Poll-server: connection closed on socket " << sender_fd << "\n";
								else
									std::cerr << "error: recv failed\n";
								close(pfds[i].fd);
								del_from_pfds(pfds, i, &fd_count);
							}
							else									// We got some good data from a client
							{
								std::cout << nbytes_recv << " bytes read on socket " << pfds[i].fd << ":\n";
								std::cout << buf << std::endl;
								int nbytes_sent = send(pfds[i].fd, response.c_str(), response.length(), 0);			// ESSAYER D'ENVOYER 1 SEULE FOIS, DES QUE LA REQUETE ENTIERE RECVEID
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
		// }
	}
	// FREE PFDS
	return 0;
}

/*
	Règles:

	- It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the client and the server (listen included).
	- poll() (or equivalent) must check read and write at the same time.
	- You must never do a read or a write operation without going through poll() (or equivalent).
*/