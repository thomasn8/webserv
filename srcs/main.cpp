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
	std::cerr << "Configs in server: "<< server.getConfigs().size() << std::endl;
	parseConfig(configFile, server);
	std::cerr << "Configs in server: "<< server.getConfigs().size() << std::endl;
	std::cerr << "Configs in server: "<< &server.getConfigs()[0] << std::endl;
	std::cerr << "Configs in server: "<< &server.getConfigs()[1] << std::endl;
	// starter le server

	return 0;
}
