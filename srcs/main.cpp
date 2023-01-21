#include "config/parse_config.hpp"
#include "../includes/Monitor.hpp"

int main(int ac, char **av, char **env)
{
	if (ac != 1 && ac != 2)
	{
		std::cerr << "usage: ./webserv [config_file]" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string configFile;
	if (ac == 2)
		configFile = av[1];
	else
		configFile = "conf/default.conf";
	Monitor monitor;
	parse_config(configFile, monitor);
	monitor.handle_connections();
	return 0;
}
