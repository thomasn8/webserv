#include "Server.hpp"

int main(int ac, char **av, char **env)
{
	if (ac != 1 || ac != 2)
	{
		perror("Usage: ./webserv [configuration_file]");
		exit(EXIT_FAILURE);
	}

	std::string configFile;
	if (ac == 2)
		configFile = av[1];
	else
		configFile = "../conf/default.conf";
	Config config(configFile);
	
	Server server(config);
	server.startServer();
	server.stopServer();
	return 0;
}
