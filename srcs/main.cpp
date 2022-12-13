#include "parse_config.hpp"
#include "Config.hpp"
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
	std::vector<Config> configs;
	Server server(configs);
	parseConfig(configFile, server, configs);
	// starter le server avec les configs enregistrees
	return 0;
}
