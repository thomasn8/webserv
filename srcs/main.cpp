#include "config/parse_config.hpp"
#include "../includes/Monitor.hpp"
#include "../includes/Server.hpp"
#include "../includes/Request.hpp"

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
	Monitor monitor;
	parse_config(configFile, monitor);
	monitor.handle_connections();

	// std::string message = "GET /contact_form.php HTTP/1.1\r\nHost: developer.mozilla.org\r\nContent-Length: 64\r\nContent-Type: application/x-www-form-urlencoded, test\r\n\r\nname=Joe%20User&request=Send%20me%20one%20of%20your%20catalogue";

	// try {
    //     Request newRequest(message);
	// 	std::cout << newRequest.get_method() << " " << newRequest.get_target() << " " << newRequest.get_version() << std::endl;
	// 	newRequest.display_fields();
	// 	std::cout << "\n" << newRequest.get_body() << std::endl;
    // }
    // catch (Request::MessageException & e) {
    //     std::cout << "Error: " << e.what() << std::endl;
    // }
	return 0;
}
