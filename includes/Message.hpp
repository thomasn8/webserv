#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <string>
#include <list>
#include <map>
#include "methods.hpp"

// Abstract class for common data 
// shared with request and responses
class Message {
	public:
		typedef std::map<std::string, std::list<std::string>>::iterator mapit;
		typedef std::list<std::string>::iterator listit;

		Message();
		Message(const Message &instance);
		virtual ~Message() = 0;

		std::map<std::string, std::list<std::string>>	get_fields() const;
		void											display_fields() const;
		std::string										get_body() const;
		bool											has_body() const;

		Message		&operator=(const Message &instance);

	protected:
		std::map<std::string, std::list<std::string>>	_fields;
		std::string										_body;    // UTILISER QUE DANS REPONSE
};

#endif
