#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "Server.hpp"
#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

#define ENCTYPE_DEFAULT 1
#define ENCTYPE_MULTIPART 2

class Request : public Message {
	public:
		Request(std::string *rowMessage, Server *server);
		Request(const Request &instance);
		virtual ~Request();

		std::string get_message() const;
		std::string get_method() const;
		std::string get_target() const;
		std::string get_version() const;

		Request		&operator=(const Request &instance);

	private:
		Request();

		Server		*_server;
		std::string *_rawMessage;
		std::string _method;
		std::string _target;
		std::string _version;
		
		std::map<std::string, std::string> _postDefault;
		// std::list<std::pair<std::string, std::string> > _postMultipart;

		void		_check_alone_CR(void);
		void		_parse_start_line(std::string startLine);
		void		_split_field(size_t separator, size_t lastchar);
		int			_parse_header();
		void		_parse_body();
		bool		_check_filetype(std::string &contentType);
};

#endif
