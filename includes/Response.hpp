#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Message.hpp"
#include "Request.hpp"

class Server;

class Response : public Message {
	public:
		// Response(Request request, Server &server);
		Response(Request request);
		// Response(const Response &instance);
		virtual ~Response();

		std::string getMessage() const;
		std::string getStatusCode() const;
		std::string getReason() const;
		std::string getVersion() const;

		Response		&operator=(const Response &instance);

	private:
		Response();

		void		_response_get();
		void		_response_post();
		void		_response_delete();
		void		_check_target(std::string target);
		
		Request		_request;
		// Server		&_server;
		std::string _finalMessage;
		std::string _version;
		std::string _statusCode;
		std::string _reason;
		bool		_isCGI;
		bool		_targetFound;
};

#endif
