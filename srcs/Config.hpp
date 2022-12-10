/*
	Ébauche/idées de classes pour gérer notre data structure dans ce projet

	PARSING DU FICHIER CONFIG DU SERVER
*/

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <netinet/in.h>

class Config
{	
	public:
		typedef unsigned long size_type;

		Config();
		~Config();
		
		uint16_t getPort() const;
		uint32_t getAddress() const;
		size_type getClientMaxBodySize() const;

	private:
		uint16_t _port;
		uint32_t _address;				// si pas specifier: INADDR_ANY
		size_type _clientMaxBodySize;

};

#endif
