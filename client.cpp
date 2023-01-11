/*
	c++ -Wall -Wextra -Werror client.cpp -o client
	./client [port]
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
# define RED "\033[0;31m"
# define BLU "\033[0;34m"
# define WHI "\033[0m"

void error(const char *msg, const char *str_errorno, int code)
{
	std::cerr << msg << str_errorno << std::endl;
	exit(code);
}

int port_check(const char *av1)
{
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

int main(int ac, const char **av)
{
	if (ac > 2)
		error("Usage: ./client [port]", "", 1);
	int port = PORT;
	if (ac == 2)
		port = port_check(av[1]);

	int socket_fd = -1;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		error("Error: socket() failed: ", strerror(errno), 1);

	struct sockaddr_in server_addr;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0)
		error("Error: inet_pton(): ", strerror(errno), 1);
	server_addr.sin_port = htons(port);
	server_addr.sin_len = sizeof(server_addr);
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
	
	int i = -1;
	std::cout << BLU << std::endl;
	while (++i < recv_size)
		write(STDOUT_FILENO, &buffer[i], 1);
	std::cout << WHI << std::endl;

	return 0;
}
