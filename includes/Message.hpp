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
		Message();
		Message(const Message &instance);
		virtual ~Message() = 0;

		std::map<std::string, std::list<std::string>>	get_fields() const;
		void											display_fields() const;
		std::string										get_body() const;
		bool											has_body() const;

		Message		&operator=(const Message &instance);

		class MessageException : public std::exception {
            public:
				MessageException(int code) : _code(std::to_string(code)) {}
                virtual const char* what() throw() {
                    return (this->_code).c_str();	
                }
			private:
				std::string _code;
        };

	protected:
		std::map<std::string, std::list<std::string>>	_fields;
		bool											_hasBody;
		std::string										_body;
};

#endif
