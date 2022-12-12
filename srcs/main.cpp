#include <sstream>
#include "Config.hpp"
#include "Server.hpp"

void exitWithError(std::ostream & stream, const std::string message, int code)
{
	stream << message;
	exit(code);
}

void parseConfig(std::string & configFile, Server & server, std::vector<Config> & configs)
{
	// ouverture du fichier
	std::ifstream configStream;
	configStream.open(configFile, std::ifstream::in);
	if (configStream.fail() == true)
		exitWithError(std::cerr, "Error while opening configuration file\n", 1);
	
	std::string line, word;
	std::istringstream iss;
	int pos;
	while (configStream)			// lecture line by line
	{
		std::getline(configStream, line);
		pos = line.find('#', 0);
		if (pos >= 0)
			line.erase(pos);
		// FAIRE DES OPERATION SUR LES LIGNES ICI
		// if (line.length())
		// 	std::cout << "[";
		iss.str(line);
		while (iss)					// lecture mot par mot
		{
			iss >> word;
			if (word.length() == 0)
				break;
			// FAIRE DES OPERATION SUR LES MOTS ICI
			std::cout << word << " ";
			word.clear();
		}
		iss.clear();
		// FAIRE DES OPERATION SUR LES LIGNES ICI
		// if (line.length())
		// 	std::cout << "] inside: " << std::endl << "\"" << line << "\"" << std::endl << std::endl;
		line.clear();
	}
	configStream.close();

	// std::string accessFile;
	// std::string errorFile;

	// _serverNames[0] = ...;
	// if (_serverNames[0]  == "localhost")
	// 	_serverNames[0]  == "127.0.0.1";
	// _address = ntohl(_serverNames[0] .c_str());
	// if (_adress == -1)
	// 	_exitWithError(std::cerr, "Error: Invalid IPv4 address\n", 1);	
}

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
	
	// tout en parsant le fichier de config
	parseConfig(configFile, server, configs);
		// checker la validite
		// ignorer
		// allouer les servers block dans des OBJ config stoker dans un vector
	
	// starter le server avec les configs enregistrees
	return 0;
}
