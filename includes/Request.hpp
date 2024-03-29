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
#include "StatusCodeException.hpp"

# define URL_MAX_LEN 2083

class Request {
	public:
		typedef std::map<std::string, std::list<std::string> >::const_iterator	fields_it;
		typedef std::list<std::string>::const_iterator 							fields_values_it;
		typedef std::list<MultipartData *>::const_iterator						mutlipart_it;

		Request(std::string *rowMessage, Server *server);
		Request(const Request &instance);
		~Request();

		std::string const & get_method() const;
		std::string const & get_target() const;
		std::map<std::string, std::list<std::string> > const &get_fields() const;
		std::string const & get_queryString() const;
		std::string const & get_postDefault() const;
		std::list<MultipartData *> const &get_multipartDatas() const;

		class RequestException : public StatusCodeException {
			public:
				RequestException(const int code) : StatusCodeException(code) {}
		};

	private:
		Request();
		
		// PARSE ALL
		std::string *_rawMessage;
		Server *_server;

		// PARSE HEADER
		std::string _method;
		std::string _target;
		std::map<std::string, std::list<std::string> > _fields;
		void _parse_start_line(std::string startLine);
		void _trim_sides(std::string &str);
		void _split_field(size_t separator, size_t lastchar);
		int _parse_header();

		// PARSE BODY
		const char *_body;
		size_t _body_len;
		void _parse_body();
		// get
		std::string _queryString;
		// post default
		std::string _postDefault;
		// parse post multipart type
		std::list<MultipartData *> _postMultipart;
		void _parse_multipartDataType(fields_it type);
		std::string _find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop);
		bool _check_filetype(std::string contentType);
		void _free_multipartDatas();

		// PRINT DATAS
		// void _print_fields() const;
		// void _print_multipartDatas() const;
};

#endif
