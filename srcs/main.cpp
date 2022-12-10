#include "Server.hpp"

int main(int ac, char **av, char **env)
{
	if (ac != 1 || ac != 2)
	{
		perror("Usage: ./webserv [configuration_file]");
		exit(EXIT_FAILURE);
	}
	Server server;
	if (ac == 2)
		server.parse_config_file(av[1]);
	else
		server.parse_config_file("../conf/default.conf");
	server.start_server();





	// while(1)
	// {
	// 	std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
	// 	client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address.sin_len);
	// 	if (client_fd < 0)
	// 	{
	// 		perror("accept failed");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	client_read = read(client_fd, buffer, 30000);
	// 	if (client_read < 0)
	// 		perror("No bytes are there to read");
	// 	std::cout << buffer << std::endl;
	// 	write(client_fd, response.c_str(), response.length());
	// 	std::cout << "------- Hello message sent -------" << std::endl;
	// 	close(client_fd);
	// }

	return 0;
}
