/*
	make client OR	c++ -Wall -Wextra -Werror client.cpp -o client
	make run-client OR ./client [...]
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

# define USAGE "Usage: ./client [-i ipv4] [-p port] [-n repeat] request[[-s arg] or [-f filename]]"
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
# define FILE 1
# define ARGUMENT 2

const char option1[] = "-i";
const char option2[] = "-p";
const char option3[] = "-n";
const char option4[] = "-s";
const char option5[] = "-f";
const char *options[5] = {option1, option2, option3, option4, option5};
const int len = 5;


struct request {
	const char	*ip;
	int			port;
	int			repeatcount;
	int 		type;
	const char	*requestArg;
	const char	*filename;
	char 		*request;
	ssize_t		size;
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

void highlight_crlf(const char *block, ssize_t size, const char *highlightcolor, const char *defaultcolor)
{
	if (highlightcolor != NULL && defaultcolor != NULL)
	{
		for (ssize_t j = 0 ; j < size ; j++)
			block[j] == '\r' ? std::cout << highlightcolor << "\\r" << defaultcolor : (block[j] == '\n' ? std::cout << highlightcolor << "\\n\n" << defaultcolor : std::cout << block[j]);
	}
	else
	{
		for (ssize_t j = 0 ; j < size ; j++)
			block[j] == '\r' ? std::cout << "\\r" : (block[j] == '\n' ? std::cout << "\\n\n" : std::cout << block[j]);
	}
}

void add_carriageReturn_to_header(struct request * test)
{
	// highlight_crlf(test->request, test->size);
	char *tmp = NULL;
	ssize_t i = 0, body = 0;
	while (i < test->size)
	{
		if (test->request[i] == '\n')
		{
			// empty line == end of header
			if (test->size > 2 && test->request[i-1] == '\n' && test->request[i-2] == '\r')
			{
				body = i + 1;
				break;
			}

			// ajouter un \r avant le \n
			tmp = test->request;
			test->request = (char *)malloc(test->size + 1);
			test->size += 1;
			memcpy(test->request, tmp, i);
			test->request[i] = '\r';
			memcpy(&test->request[i+1], &tmp[i], test->size-i-1);
			free(tmp);

			i++; // shift le \r pour rester sur le \n detecté
		}
		i++;
	}

	if (body == 0)
	{
		if (test->request[test->size - 1] != '\n')
		{
			test->request = (char *)realloc(test->request, test->size + 4);
			test->size += 4;
		}
		else
		{
			test->request = (char *)realloc(test->request, test->size + 3);
			test->size += 3;
		}
		test->request[test->size-4] = '\r';
		test->request[test->size-3] = '\n';
		test->request[test->size-2] = '\r';
		test->request[test->size-1] = '\n';
	}
	else
	{
		tmp = test->request;
		test->request = (char *)malloc(test->size + 1);
		test->size += 1;
		memcpy(test->request, tmp, body);
		test->request[body-1] = '\r';
		std::string teststr( &tmp[body-1], test->size-body);
		memcpy(&test->request[body], &tmp[body-1], test->size-body);
		free(tmp);
	}
	// highlight_crlf(test->request, test->size);
}

// ameliorations: ajoute la possibilite de passer des args sans specifier l'option pour le port et pour le requestArg
// exemple: ./client 8080 "GET /index.html HTTP/1.1"
void parse(int ac, const char **av, struct request * test) {
	if (ac > len*2-1)
		error(USAGE, "", 1);
	test->ip = IP;
	test->port = PORT;
	test->repeatcount = 1;
	test->type = 0;
	test->requestArg = NULL;
	test->filename = NULL;
	test->request = NULL;
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
								test->ip = av[i+1];
							j = len+1;				// breaks while loop
							break;
						case 1:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->port = port_check(av[i+1]);
							j = len+1;
							break;
						case 2:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->repeatcount = atoi(av[i+1]);
							j = len+1;
							break;
						case 3:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->requestArg = av[i+1];
							j = len+1;
							break;
						case 4:
							if (ac < i+2)
								error("Error: incomplete option ", av[i], 1);
							else
								test->filename = av[i+1];
							j = len+1;
							break;
					}
				}
			}
		}
	}
	if (test->repeatcount < 1)
		error("Error: repeatcount must be set at least to 1 in order to send any request", "", 1);
	if (test->requestArg == NULL && test->filename == NULL)
		error("Error: no request to send: either -f or -s option must be specified\n", USAGE, 1);
	if (test->requestArg != NULL && test->filename != NULL)
		error("Error: either -f or -s option must be specified, not both\n", USAGE, 1);
	if (test->filename != NULL) {
		test->type = FILE;
		// open file
		std::ifstream ifs(test->filename, std::ifstream::binary);
		if (!ifs.is_open())
    		error("Error: impossible to open input file", "", 1);
		// get pointer to associated buffer object
		std::filebuf *pbuf = ifs.rdbuf();
		// get file size using buffer's members
		size_t size = pbuf->pubseekoff(0,ifs.end,ifs.in);
		pbuf->pubseekpos (0,ifs.in);
		if (size > FILE_MAX_LEN)
			error("Error: input file is too large: maximum is 1MO", "", 1);
		// allocate memory to contain file data
		test->request = new char[size];
		test->size = size;
		// get file data
		pbuf->sgetn(test->request, size);
		ifs.close();
	}
	else
	{
		test->type = ARGUMENT;
		test->size = strlen(test->requestArg);
		test->request = (char *)malloc(test->size);
		memcpy(test->request, test->requestArg, test->size);
	}
	add_carriageReturn_to_header(test);
}

int main(int ac, const char **av) {

	// PARSE ARGS
	struct request test;
	parse(ac, av, &test);
	std::cout << "Summary\nYou asked to request " << test.repeatcount << " times on ip " << test.ip << ":" << test.port << " with the message:\n";
	std::cout << BLU;
	highlight_crlf(test.request, test.size, WHI, BLU);
	std::cout << WHI << "\nResult" << std::endl;

	// SERVER INFOS
	struct sockaddr_in server_addr;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, test.ip, &server_addr.sin_addr) <= 0)
		error("Error: inet_pton(): ", strerror(errno), 1);
	server_addr.sin_port = htons(test.port);
	server_addr.sin_len = sizeof(server_addr);

	// REQUESTS n TIMES THE SERVER (depends on args)
	ssize_t recv_size;
	char buffer[BUFFER_SIZE];
	for (int i = 0; i < test.repeatcount; i++) {
		// GET FD
		int socket_fd = -1;
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0)
		error("Error: socket() failed: ", strerror(errno), 1);

		// CONNECT
		if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
			error("Error: connect(): ", strerror(errno), 1);

		// SEND
		ssize_t send_size = send(socket_fd, test.request, test.size, 0);
		if (send_size == test.size)
			std::cout << "Success: request sent\n" << std::endl;
		else if (send_size > 0)
			std::cout << "Error: request partially sent\n" << std::endl;
		else
			error("Error: send() failed: ", strerror(errno), 1);

		// RECV
		recv_size = recv(socket_fd, buffer, BUFFER_SIZE, 0);
		if (recv_size > 0)
			std::cout << "Response received:";
		else if(recv_size == 0)
			std::cout << "Empty response received\n";
		else
			error("Error: recv() failed: ", strerror(errno), 1);
		
		// CLOSE
		close(socket_fd);
	}

	// PRINT ONLY LAST RESPONSE	(THATS WHY OUTSIDE OF LOOP)
	int i = -1;
	std::cout << BLU << std::endl;
	while (++i < recv_size)
		write(STDOUT_FILENO, &buffer[i], 1);
	std::cout << WHI << std::endl;

	if (test.type == FILE)
		delete[] test.request;
	else if (test.type == ARGUMENT)
		free(test.request);
	return 0;
}
