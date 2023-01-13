#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "debug.hpp"
#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

class Request : public Message {
	public:
		Request(std::string *rowMessage);
		Request(const Request &instance);
		virtual ~Request();

		std::string get_message() const;
		std::string get_method() const;
		std::string get_target() const;
		std::string get_version() const;

		Request		&operator=(const Request &instance);

	private:
		Request();

		std::string *_rawMessage;
		std::string _method;
		std::string _target;
		std::string _version;
		
		void		_check_alone_CR(void);
		void		_parse_start_line(std::string startLine);
		// void		_split_field(size_t pos, std::string line);
		void		_split_field(size_t separator, size_t lastchar);
		// void		_parse_header(std::istringstream &raw);
		int			_parse_header();
		// void		_parse_body(std::istringstream &raw);
		void		_parse_body();
};

#endif
