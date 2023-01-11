/*
	make client OR	c++ -Wall -Wextra -Werror client.cpp -o client
	make run-client OR ./client [...]

	usage: ./client [-p port] [-n repeatcount] [[-f requestfile]/[-s requeststring]]
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

# define IP "127.0.0.1"
# define PORT 80
# define BUFFER_SIZE 1024
# define EQUAL 0
# define RED "\033[0;31m"
# define BLU "\033[0;34m"
# define WHI "\033[0m"

struct tester {
	int			port;
	size_t		repeatcount;
	std::string	file;
	std::string	request;
};

void error(const char *msg, const char *precision, int code) {
	std::cerr << msg << precision << std::endl;
	exit(code);
}

int port_check(const char *av1) {
	int port = PORT;
	std::string str = av1;
	size_t idx;

	try {
		port = std::stoi(str, &idx);
		if (str.substr(idx).size() != 0)
			error("Error: port invalid", "", 1);
	}
	catch (const std::invalid_argument &ia) {
		error("Error: port invalid", "", 1);
	}
	return port;
}

void parse(int ac, const char **av, struct tester &tester) {
	if (ac > 7)
		error("Usage: ./client [-p port] [-n repeatcount] [[-f requestfile]/[-s requeststring]]", "", 1);
	tester->port = PORT;
	tester->repeatcount = 1;
	if (ac == 1)
		return;
	char options["-p", "-n", "-f", "-s"];
	int len = 4;
	for (int i = 1; i < ac; i++) { 		// itere sur tous les av (sauf le prog)
		if ((i % 2) > 0) {				// si i est impair compare av aux options
			int j = -1;
			while (++j <= len) {				// itere sur toutes les options
				if (j == len)					// erreur: pas d'option correspondante
					error("Error: invalid option ", av[i], 1);
				if (strncmp(av[i], options[j], strlen(av[i])) == EQUAL) {
					// exemples:

					// ./client   -p  
					// av[1] == options[0]: ac == 2, i == 1
					// erreur: ac minimum == i + 2 == 3
					
					// ./client   -p  80
					// av[1] == options[0]: ac == 3, i == 1
					// ok: ac minimum == i + 2 == 3
					switch (j)
					{
						case 0:
							if (ac < i+2) // test si av[i+1] existe
								error("Error: incomplete option ", av[i], 1);
							else
								tester->port = port_check(av[i+1]);
							break;
						case 1:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								tester->repeatcount = atoi(av[i+1]);
							break;
						case 2:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								tester->file = av[i+1]);
							break;
						case 3:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								tester->port = port_check(av[i+1]);
							break;
					}
				}
			}
		}
	}
}

int main(int ac, const char **av) {

	struct tester;
	parse(ac, av, &tester);

	int socket_fd = -1;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		error("Error: socket() failed: ", strerror(errno), 1);

	struct sockaddr_in server_addr;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0)
		error("Error: inet_pton(): ", strerror(errno), 1);
	server_addr.sin_port = htons(tester.port);
	server_addr.sin_len = sizeof(server_addr);

	// REQUESTS repeatcount THE SERVER (localhost:port)
	for (int i = 0; i < tester.repeatcount; i++) {
		if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
			error("Error: connect(): ", strerror(errno), 1);

		std::string httpMessage = "Hello from client";
		ssize_t send_size = send(socket_fd, httpMessage.c_str(), httpMessage.size(), 0);
		if (send_size > 0)
			std::cout << "\nRequest sent:\n\n" << RED << httpMessage << WHI << std::endl;
			
		char buffer[BUFFER_SIZE];
		ssize_t recv_size = recv(socket_fd, buffer, BUFFER_SIZE, 0);
		if (recv_size > 0)
			std::cout << "\nResponse received:\n";
	}

	// PRINT ONLY LAST RESPONSE	
	int i = -1;
	std::cout << BLU << std::endl;
	while (++i < recv_size)
		write(STDOUT_FILENO, &buffer[i], 1);
	std::cout << WHI << std::endl;

	return 0;
}
