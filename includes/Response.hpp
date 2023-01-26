#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <fstream>
#include <streambuf>
#include <unistd.h>
#include "Message.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "debug.hpp"

class Response : public Message {
	public:
		Response(std::string code, Server *server, std::string * finalMessage);
		Response(Request *request, Server *server, std::string * finalMessage);
		Response(const Response &instance);
		virtual ~Response();

		std::string getMessage() const;
		std::string getStatusCode() const;
		std::string getReason() const;
		std::string getVersion() const;

		Response	&operator=(const Response &instance);

	private:
		Response();

		void		_response_get();
		void		_response_post();
		void		_response_delete();
		void		_error_messages();
		int			_check_error_pages(std::string code);
		void		_check_target_in_get(std::string target);
		int			_check_redirections(std::string &target, std::deque<Location> &locations);
		int			_check_redirections_directory(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
		void		_add_root_to_target(std::string &target, std::deque<Location> &locations);
		int			_is_index_file(std::string &target, std::list<std::string> indexes);
		void		_check_locations(std::string &target, std::deque<Location> &locations);
		void		_check_locations_directory(std::string &target, std::deque<Location> &locations, std::deque<Location>::iterator &locationFound);
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
		bool 						_autoindex;
		bool						_isCGI;
		bool						_targetFound;
};

#endif
