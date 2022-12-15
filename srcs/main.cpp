#include "config/parse_config.hpp"
#include "config/Config.hpp"
#include "Server.hpp"

int main(int ac, char **av, char **env)
{
	if (ac != 1 && ac != 2)
	{
		perror("Usage: ./webserv [configuration_file]");
		exit(EXIT_FAILURE);
	}
	std::string configFile;
	if (ac == 2)
		configFile = av[1];
	else
		configFile = "conf/default.conf";
	Server server;
	parse_config(configFile, server);

	// TESTER LE PARSE 
		// checker les Config object si bien remplis
	// ...

	// starter le server
	return 0;
}
