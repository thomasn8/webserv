#include "config/parse_config.hpp"
#include "config/Config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Request.hpp"

int main(int ac, char **av, char **env)
{
	if (ac != 1 && ac != 2)
	{
		perror("Usage: ./webserv [configuration_file]");
		exit(EXIT_FAILURE);
	}
	// std::string configFile;
	// if (ac == 2)
	// 	configFile = av[1];
	// else
	// 	configFile = "conf/default.conf";
	// Server server;
	// parse_config(configFile, server);

	//Request and responses
	std::string message = "GET \r/contact_form.php HTTP/1.1 truc\r\nHost: developer.mozilla.org\nContent-Length: 64\nContent-Type: application/x-www-form-urlencoded\n\nname=Joe%20User&request=Send%20me%20one%20of%20your%20catalogue";

	try {
        Request newRequest(message);
    }
    catch (Request::MessageException & e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
	// starter le server
	return 0;
}
