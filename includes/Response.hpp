#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Message.hpp"
#include "Request.hpp"

class Response : public Message {
	public:
		Response(Request &request);
		Response(const Response &instance);
		virtual ~Response();

		Response		&operator=(const Response &instance);

	private:
		Response();

		Request		&_request;
		std::string _rowMessage;
		std::string _method;
		std::string _target;
		std::string _version;
};

#endif
