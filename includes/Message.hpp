#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>
#include <utility>
#include <map>

// methods to implement
# define GET "GET"
# define POST "POST"
# define DELETE "DELETE"

// Abstract class for common data 
// shared with request and responses
class Message {
	public:
		Message();
		Message(const Message &instance);
		virtual ~Message() = 0;

		Message		&operator=(const Message &instance);

	protected:
		std::map<std::string, std::string>	_fields;
		bool								_hasBody;
		std::string							_body;
};

#endif
