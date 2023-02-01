#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <fstream>
#include <streambuf>
#include <unistd.h>
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "debug.hpp"
#include "StatusCodeException.hpp"

class Response {
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

		void			_response_get();
		void			_response_post();
		void			_response_delete();
		void			_status_messages();
		int				_check_error_pages(const int code);
		void			_check_target();
		int				_check_redirections(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
		void 			_check_methods_in_location(std::deque<Location>::iterator &locationFound);
		// void			_add_root_to_target(std::string &target, std::deque<Location> &locations);
		int				_add_root_if_cgi(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
		int				_is_index_file(std::string &target, std::list<std::string> indexes);
		void			_check_locations(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
		void			_check_locations_directory(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
		int				_make_CGI();
		void			_make_response();
		void			_decript_img();
		std::string		_what_kind_of_cgi(std::string &target);
		std::string		_what_kind_of_extention(std::string &target);
		
		Request						*_request;
		Server						*_server;
		std::map<int, std::string> 	_statusMsg;
		std::string 				*_finalMessage;
		std::string 				_version;
		std::string 				_header;
		std::string 				_statusCode;
		std::string 				_reason;
		std::string 				_target;
		std::string 				_targetType;
		std::string 				_uploadsDir;
		std::list<std::string>		_contentType;
		std::string					_cgi;
		bool 						_autoindex;
		bool						_targetFound;
};

#endif
