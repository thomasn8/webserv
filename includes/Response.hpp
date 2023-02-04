#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "debug.hpp"
#include "StatusCodeException.hpp"

class Response {
	public:
		Response(const int code, Server *server, char **responseStr, size_t *responseSize);
		Response(char **env, Request *request, Server *server, char **responseStr, size_t *responseSize);
		Response(const Response &instance);
		virtual ~Response();

		char	**_finalMessage;
		size_t	*_finalMessageSize;

		char * getFinaleMessage() const;
		size_t getFinaleMessageSize() const;
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
		std::string 	_status_messages(int code);
		int				_check_error_pages(const int code);
		void			_check_target();
		int				_check_redirections(std::string &target, std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound);
		void 			_check_methods_in_location(std::deque<Location>::const_iterator &locationFound);
		int				_add_root_if_cgi(std::string &target, std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound);
		int				_is_index_file(std::string &target, std::list<std::string> const &indexes);
		void			_check_locations(std::string &target, std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound);
		void			_check_locations_directory(std::string &target, std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound);
		int				_make_CGI();
		void			_check_body();
		void			_upload_file(MultipartData *data);
		void			_make_autoindex();
		void			_make_response();
		void			_execute_cgi();
		char**			_prepare_env();
		std::string		_what_kind_of_cgi(std::string &target);
		std::string		_what_kind_of_extention(std::string &target);
		void 			_make_final_message(std::string &header, const char *body, std::filebuf *pbuf, size_t len);
		
		Request						*_request;
		Server						*_server;
		std::string 				_version;
		std::string 				_header;
		std::string 				_statusCode;
		std::string 				_reason;
		std::string 				_target;
		std::string 				_targetType;
		std::string 				_uploadsDir;
		std::list<std::string>		_contentType;
		std::string					_cgi;
		std::string					_body;
		std::string					_contentLength;
		bool 						_autoindex;
		bool						_targetFound;
		char						**_env;
};

#endif
