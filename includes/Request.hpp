#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"
#include "Message.hpp"
#include "StatusCode.hpp"
#include "Server.hpp"
#include "MultipartData.hpp"
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <map>
#include <list>

class Request : public Message {
	public:
		Request(std::string *rowMessage, Server *server);
		Request(const Request &instance);
		virtual ~Request();

		std::string get_message() const;
		std::string get_method() const;
		std::string get_target() const;
		std::string get_version() const;
		std::map<std::string, std::string> &get_defaultDatas();
		std::list<MultipartData *> &get_multipartDatas();

		Request		&operator=(const Request &instance);

	private:
		Request();

		Server		*_server;
		std::string *_rawMessage;
		
		// parse all
		std::string _method;
		std::string _target;
		std::string _version;
		void	_check_alone_CR(void);
		void	_parse_start_line(std::string startLine);
		void	_split_field(size_t separator, size_t lastchar);
		int		_parse_header();
		void	_parse_body();

		// parse default type
		std::map<std::string, std::string> _postNameValue;
		void	_parse_defaultDataType();
		void	_print_defaultDatas() const;

		// parse multipart type
		std::list<MultipartData *> _postMultipart;
		void	_parse_multipartDataType(mapit type);
		std::string _find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop);
		bool 	_check_filetype(std::string contentType);
		void	_print_multipartDatas() const;
		void	_free_multipartDatas();
};

#endif
