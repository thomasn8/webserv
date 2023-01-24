#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <list>
#include <exception>

#include "utils.hpp"
#include "debug.hpp"
#include "StatusCode.hpp"
#include "Server.hpp"
#include "MultipartData.hpp"

class Request {
	public:
		typedef std::map<std::string, std::list<std::string>>::const_iterator	fields_it;
		typedef std::list<std::string>::const_iterator 							fields_values_it;
		typedef std::map<std::string, std::string>::const_iterator				post_mapit;
		typedef std::list<MultipartData *>::const_iterator						post_listit;

		Request(std::string *rowMessage, Server *server);
		Request(const Request &instance);
		~Request();

		std::string get_method() const;
		std::string get_target() const;
		std::string get_version() const;
		std::map<std::string, std::list<std::string>> get_fields() const;
		std::map<std::string, std::string> &get_defaultDatas();
		std::list<MultipartData *> &get_multipartDatas();

		class RequestException : public std::exception {
			public:
				RequestException(int code) : _code(std::to_string(code)) {}
				virtual const char* what() throw() {
					return (_code).c_str();	
				}
			private:
				std::string _code;
		};

	private:
		Request();

		std::string *_rawMessage;
		Server *_server;
		
		// parse all
		std::string _method;
		std::string _target;
		const char *_body;
		size_t _body_len;
		std::map<std::string, std::list<std::string> > _fields;
		void _parse_start_line(std::string startLine);
		void _split_field(size_t separator, size_t lastchar);
		void _trim_sides(std::string &str);
		int _parse_header();
		void _parse_body();
		void _display_fields() const;

		// parse default type
		std::map<std::string, std::string> _postNameValue;
		void _parse_defaultDataType(std::string *formDatas);
		void _print_defaultDatas() const;

		// parse multipart type
		std::list<MultipartData *> _postMultipart;
		void _parse_multipartDataType(fields_it type);
		std::string _find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop);
		bool _check_filetype(std::string contentType);
		void _print_multipartDatas() const;
		void _free_multipartDatas();
};

#endif
