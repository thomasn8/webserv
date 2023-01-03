#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Message.hpp"

class Request : public Message {
	public:
		Request(std::string rowMessage);
		Request(const Request &instance);
		virtual ~Request();

		Request		&operator=(const Request &instance);

	private:
		Request();

		std::string _rowMessage;
		std::string _method;
		std::string _target;
		std::string _version;
};

#endif
