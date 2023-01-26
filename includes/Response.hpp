#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <sys/wait.h>
#include "Message.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "debug.hpp"
#include "StatusCodeException.hpp"

class Response : public Message {
	public:
		Response(const int code, Server *server, std::string * finalMessage);
		Response(Request *request, Server *server, std::string * finalMessage);
		Response(const Response &instance);
		virtual ~Response();

		std::string getMessage() const;
		std::string getStatusCode() const;
		std::string getReason() const;
		std::string getVersion() const;

		Response	&operator=(const Response &instance);

		class ResponseException : public StatusCodeException {
			public:
				ResponseException(const int code) : StatusCodeException(code) {}
		};

	private:
		Response();

		void		_response_get();
		void		_response_post();
		void		_response_delete();
		void		_error_messages();
		int			_check_error_pages(const int code);
		void		_check_target_in_get(std::string target);
		int			_check_redirections(std::string &target, std::deque<Location> &locations);
		void		_check_locations(std::string &target, std::deque<Location> &locations);
		void		_check_root(std::string &target);
		int			_make_CGI();
		void		_make_response();
		void		_decript_img();
		
		Request						*_request;
		Server						*_server;
		std::map<int, std::string> 	_errorMsg;
		std::string 				*_finalMessage;
		std::string 				_version;
		std::string 				_header;
		std::string 				_statusCode;
		std::string 				_reason;
		std::string 				_path;
		bool						_isCGI;
		bool						_targetFound;
};

#endif
