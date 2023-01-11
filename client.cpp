/*
	make client OR	c++ -Wall -Wextra -Werror client.cpp -o client
	make run-client OR ./client [...]

	usage: ./client [-p port] [-n repeatcount] [[-f requestfile] or [-s requeststring]]
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

# define IP "127.0.0.1"
# define PORT 80
# define MIN_PORT_NO 1024
# define MAX_PORT_NO 65535
# define FILE_MAX_LEN 1000000 // 1 MO
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
	char 		*requestbuf;
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
	if (port != 80 && port < MIN_PORT_NO)
		error("Error: invalid port: port number below 1024 (except default port 80) are not available", "", 1);
	if (port > MAX_PORT_NO)
		error("Error: invalid port: maximum port number is 65535", "", 1);
	return port;
}

void parse(int ac, const char **av, struct tester * test) {
	if (ac > 7)
		error("Usage: ./client [-p port] [-n repeatcount] [[-f requestfile] or [-s requeststring]]", "", 1);
	test->port = PORT;
	test->repeatcount = 1;
	if (ac == 1)
		return;
	const char option1[] = "-p";
	const char option2[] = "-n";
	const char option3[] = "-f";
	const char option4[] = "-s";
	const char *options[4] = {option1, option2, option3, option4};
	int len = 4;
	for (int i = 1; i < ac; i++) { 		// itere sur tous les av (sauf le prog)
		if ((i % 2) > 0) {				// si i est impair compare av aux options
			int j = -1;
			while (++j <= len) {			// itere sur toutes les options
				if (j == len)				// erreur: pas d'option correspondante
					error("Error: invalid option ", av[i], 1);
				if (strncmp(av[i], options[j], strlen(av[i])) == EQUAL) {
					switch (j)
					{
						case 0:					// si match l'option n
							if (ac < i+2) 			// test si av[i+1] existe
								error("Error: incomplete option ", av[i], 1);
							else
								test->port = port_check(av[i+1]);
							break;
						case 1:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->repeatcount = atoi(av[i+1]);
							break;
						case 2:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->file = av[i+1];
							break;
						case 3:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->request = av[i+1];
							break;
					}
				}
			}
		}
	}
	if (test->repeatcount < 1)
		error("Error: repeatcount must be set at least to 1 in order to send any request", "", 1);
	if (test->file.empty() && test->request.empty())
		error("Error: no request to send: either -f or -s option must be specified", "", 1);
	if (!test->file.empty() && !test->request.empty())
		error("Error: either -f or -s option must be specified, not both", "", 1);

	if (!test->file.empty()) {
		// open file
		std::ifstream ifs(test->file.c_str(), std::ifstream::binary);
		// get pointer to associated buffer object
		std::filebuf *pbuf = ifs.rdbuf();
		// get file size using buffer's members
		size_t size = pbuf->pubseekoff(0,ifs.end,ifs.in);
		pbuf->pubseekpos (0,ifs.in);
		if (size > FILE_MAX_LEN)
			error("Error: input file is too large: maximum is 1MO", "", 1);
		// allocate memory to contain file data
		test->requestbuf = new char[size];
		// get file data
		pbuf->sgetn(test->requestbuf, size);
		ifs.close();
	}
	else
		test->requestbuf = test->request.c_str();
}

int main(int ac, const char **av) {

	struct tester test;
	parse(ac, av, &test);

	int socket_fd = -1;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		error("Error: socket() failed: ", strerror(errno), 1);

	struct sockaddr_in server_addr;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0)
		error("Error: inet_pton(): ", strerror(errno), 1);
	server_addr.sin_port = htons(test.port);
	server_addr.sin_len = sizeof(server_addr);

	// REQUESTS repeatcount THE SERVER (localhost:port)
	for (int i = 0; i < test.repeatcount; i++) {
		if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
			error("Error: connect(): ", strerror(errno), 1);

		ssize_t send_size = send(socket_fd, test.request.c_str(), test.size(), 0);
		if (send_size == test.size())
			std::cout << "\nSuccess: request sent\n" << std::endl;
		else if (send_size > 0)
			std::cout << "\nError: request partially sent\n" << std::endl;
		else
			error("Error: send() failed: ", strerror(errno), 1);

		char buffer[BUFFER_SIZE];
		ssize_t recv_size = recv(socket_fd, buffer, BUFFER_SIZE, 0);
		if (recv_size > 0)
			std::cout << "\nResponse received:\n";
	}

	// PRINT ONLY LAST RESPONSE	(THATS WHY OUTSIDE OF LOOP)
	int i = -1;
	std::cout << BLU << std::endl;
	while (++i < recv_size)
		write(STDOUT_FILENO, &buffer[i], 1);
	std::cout << WHI << std::endl;

	if (!test.file.empty())
		delete[] test.requestbuf;
	return 0;
}
