/*
	make client OR	c++ -Wall -Wextra -Werror client.cpp -o client
	make run-client OR ./client [...]
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

# define USAGE "Usage: ./client [-i ipv4] [-p port] [-n repeat] request[[-s arg] or [-f filename]]"
# define IP "127.0.0.1"
# define PORT 80
# define MIN_PORT_NO 1024
# define MAX_PORT_NO 65535
# define FILE 1
# define ARGUMENT 2
# define FILE_MAX_LEN 1000000 // 1 MO
# define POLLTIMEOUT_MS -1
// # define CHUNK_SEND 8192 // efficace mais utilisable que en localhost a cause: "Network layer imposes a maximum packet size called the maximum transmission unit (MTU)" = ~1500 bytes
# define CHUNK_SEND 1024 // ok pour MTU
// # define CHUNK_SEND 256 // faire de multiple appels a send avec un chunk faible cause des problemes de perte de connexion ou de donnees transmises
# define CHUNK_RECV 1024
// # define CHUNK_RECV 512
# define BUFFER_LIMIT 200000 // 200KO
# define MAXRECV 100000000 // 100MO
# define EQUAL 0
# define RED "\033[0;31m"
# define BLU "\033[0;34m"
# define WHI "\033[0m"

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

struct buffer_read {
	char *begin;
	char *current;
	size_t size;
	size_t capacity;
};

void error(const char *msg, const char *precision, int code) {
	std::cerr << msg << precision << std::endl;
	exit(code);
}

int port_check(const char *arg) {
	int port = atoi(arg);
	if (port == 0)
		error("Error: port invalid", "", 1);
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

void print_chunk(const char *chunk, ssize_t size, const char *color)
{
	if (color != NULL)
		std::cout << color;
	for (ssize_t j = 0 ; j < size ; j++)
		std::cout << chunk[j];
	std::cout << WHI;
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
		memcpy(&test->request[body], &tmp[body-1], test->size-body);
		free(tmp);
	}
	// highlight_crlf(test->request, test->size);
}

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

ssize_t send_all(int fd, const char * message, ssize_t size, struct pollfd *pfds, bool last, int request_no)
{
	ssize_t size_sent = 0, total_sent = 0;
	int ready;
	if (size < CHUNK_SEND)	// cas où message initiale fait < CHUNK_SEND
	{
		ready = poll(pfds, 1, POLLTIMEOUT_MS);
		if (pfds[0].revents & POLLHUP)
		{
			std::cerr << "\nError: Connection lost while sending request no " << request_no << std::endl;
			return total_sent;
		}
		if (ready == 0)
			error("Error: poll(1) timeout", "", 1);
		else if (ready == -1)
			error("Error: poll(1) failed: ", strerror(errno), 1);
		else if (ready > 0 && pfds[0].revents & POLLOUT)
		{
			size_sent = send(fd, message, size, 0);
			if (size_sent < 0)
				error("Error: send(1) failed", strerror(errno), 1);
			// std::cout << RED << "\nsent(1) " << size_sent << " bytes" << WHI << std::endl;
			// highlight_crlf(message, size_sent, WHI, BLU);
			if (last)
				print_chunk(message, size_sent, BLU);
			size -= size_sent;
			message += size_sent;
			total_sent += size_sent;			
		}
	}
	while (size > CHUNK_SEND && size_sent != -1) // cas où message initiale > CHUNK_SEND
	{
		ready = poll(pfds, 1, POLLTIMEOUT_MS);
		if (pfds[0].revents & POLLHUP)
		{
			std::cerr << "\nError: Connection lost while sending request no " << request_no << std::endl;
			return total_sent;
		}
		if (ready == 0)
			error("Error: poll(2) timeout", "", 1);
		else if (ready == -1)
			error("Error: poll(2) failed: ", strerror(errno), 1);
		else if (ready > 0 && pfds[0].revents & POLLOUT)
		{
			size_sent = send(fd, message, CHUNK_SEND, 0);
			if (size_sent < 0)
				error("Error: send(2) failed", strerror(errno), 1);
			// std::cout << RED << "\nsent(2) " << size_sent << " bytes" << WHI << std::endl;
			// highlight_crlf(message, size_sent, WHI, BLU);
			if (last)
				print_chunk(message, size_sent, BLU);
			size -= size_sent;
			message += size_sent;
			total_sent += size_sent;
		}
	}
	while (size > 0 && size_sent != -1) // envoie les derniers bytes lorsque message initiale était > CHUNK_SEND bytes ou lorsque send a pas fonctionné comme prévu
	{
		ready = poll(pfds, 1, POLLTIMEOUT_MS);
		if (pfds[0].revents & POLLHUP)
		{
			std::cerr << "\nError: Connection lost while sending request no " << request_no << std::endl;
			return total_sent;
		}
		if (ready == 0)
			error("Error: poll(3) timeout", "", 1);
		else if (ready == -1)
			error("Error: poll(3) failed: ", strerror(errno), 1);
		else if (ready > 0 && pfds[0].revents & POLLOUT)
		{
			size_sent = send(fd, message, size, 0);
			if (size_sent < 0)
				error("Error: send(3) failed", strerror(errno), 1);
			// std::cout << RED << "\nsent(3) " << size_sent << " bytes" << WHI << std::endl;
			// highlight_crlf(message, size_sent, WHI, BLU);
			if (last)
				print_chunk(message, size_sent, BLU);
			size -= size_sent;
			message += size_sent;
			total_sent += size_sent;
		}
	}
	return total_sent;
}

ssize_t recv_all(int fd, struct buffer_read *buf, bool last)
{
	ssize_t size_recv = 0;
	buf->size = 0;
	buf->current = buf->begin;
	while (1)
	{
		if (buf->size + CHUNK_RECV > buf->capacity)
		{
			if (buf->capacity == 0)
			{
				buf->begin = (char *)malloc(CHUNK_RECV);
				buf->capacity = CHUNK_RECV;
			}
			else
			{
				buf->begin = (char *)realloc(buf->begin, buf->capacity * 2);
				buf->capacity *= 2;
			}
			if (buf->begin == NULL)
				error("Error: allocation for read buffer failed: ", strerror(errno), 1);
			buf->current = buf->begin + buf->size;
		}
		size_recv = recv(fd, buf->current, CHUNK_RECV, 0);
		if (last == true)
		{
			std::cout << BLU;
			for (ssize_t i = 0; i < size_recv; i++)
				std::cout << buf->current[i];
		}
		buf->size += size_recv;
		buf->current += size_recv;
		if (buf->size > MAXRECV)
			return -1;
		if (size_recv < CHUNK_RECV) // toute la request a été read
			return buf->size;
	}
}

// ameliorations:
// - ajouter la possibilite de passer des args sans specifier l'option pour le port et pour le requestArg
// exemple: ./client 8080 "GET /index.html HTTP/1.1"
// - ajouter un poll pour verifier si on peut manier le socket_fd 
//		- entre connect() et send()
//		- entre send et recv()
int main(int ac, const char **av) {

	// PARSE ARGS
	struct request test;
	parse(ac, av, &test);
	std::cout << "Summary\nYou asked to send " << test.repeatcount << " request(s) on ip " << test.ip << ":" << test.port << std::endl;

	// SERVER INFOS
	struct sockaddr_in server_addr;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, test.ip, &server_addr.sin_addr) <= 0)
		error("Error: inet_pton(): ", strerror(errno), 1);
	server_addr.sin_port = htons(test.port);
	server_addr.sin_len = sizeof(server_addr);

	// REQUESTS n TIMES THE SERVER (depends on args)
	struct timeval timeout;      
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	struct pollfd pfds[1];
	ssize_t send_size, recv_size;
	struct buffer_read buf;
	buf.capacity = 0;
	bool last = false;
	for (int i = 0; i < test.repeatcount; i++) {
		i == test.repeatcount-1 ? last = true : false; // print just last response

		// SOCKET
		int socket_fd = -1;
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		fcntl(socket_fd, F_SETFL, O_NONBLOCK);
		if (socket_fd < 0)
			error("Error: socket() failed: ", strerror(errno), 1);
		if (setsockopt (socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
			error("Error: setsockopt(1) failed: ", strerror(errno), 1);
		if (setsockopt (socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
			error("Error: setsockopt(2) failed: ", strerror(errno), 1);

		// CONNECT
		connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		// if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
			// error("Error: connect(): ", strerror(errno), 1);
			// 	error("Error: connect(): ", std::to_string(errno).c_str(), 1);
		pfds[0].fd = socket_fd;
    	pfds[0].events = POLLOUT;
    	pfds[0].revents = 0;

		// SEND
		if (test.repeatcount > 1 && last)
			std::cout << "\nLAST EXCHANGE:" << std::endl << std::endl;
		send_size = send_all(socket_fd, test.request, test.size, pfds, last, i+1);
		if (send_size == test.size)
			std::cout << RED << "Success: " << send_size << " bytes sent" << WHI << std::endl;
		else if (send_size > 0) // >= 0
		{
			std::cout << "Request partially sent: " << send_size << " / " << test.size << " bytes" << std::endl;
			exit(1);
		}
		else
			error("Error: send() failed: ", strerror(errno), 1);
		pfds[0].events = POLLIN;

		// RECV
		poll(pfds, 1, -1);
		if (pfds[0].revents & POLLIN)
		{
			if (last)
				std::cout << std::endl;
			recv_size = recv_all(socket_fd, &buf, last);
			if (last)
				std::cout << std::endl;
			if (recv_size > -1)
				std::cout << RED << recv_size << " bytes received in total" << WHI << std::endl;
			else
				error("Error: recv() failed: ", strerror(errno), 1);
		}
		
		// FREE RESPONSE MEMORY
		if (buf.capacity > BUFFER_LIMIT)
		{
			free(buf.begin);
			buf.capacity = 0;
		}

		// CLOSE SOCKET 
		close(socket_fd);
	}
	
	// FREE REQUEST MEMORY
	if (test.type == FILE)
		delete[] test.request;
	else if (test.type == ARGUMENT)
		free(test.request);
	return 0;
}


/*	Notes a propos de l'implementation des sockets

	connect:
	If the connection cannot be established immediately and O_NONBLOCK is set for the file descriptor for the socket, 
	connect() shall fail and set errno to [EINPROGRESS], but the connection request shall not be aborted, and the connection shall be established asynchronously.
	Subsequent calls to connect() for the same socket, before the connection is established, shall fail and set errno to [EALREADY].
	When the connection has been established asynchronously, select() and poll() shall indicate that the file descriptor for the socket is ready for writing.
	EINPROGRESS = 36
	EALREADY = 37

	send :
	If space is not available at the sending socket to hold the message to be transmitted, and the socket file descriptor does not have O_NONBLOCK set, send() shall block until space is available. 
	If space is not available at the sending socket to hold the message to be transmitted, and the socket file descriptor does have O_NONBLOCK set, send() shall fail. 
	The select() and poll() functions can be used to determine when it is possible to send more data.
	
	poll :
	If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait
	for any file descriptor to become ready.  If timeout is zero, then poll() will return
	without blocking. If the value of timeout is -1, the poll blocks indefinitely.
*/