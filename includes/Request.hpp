#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include <map>
#include <list>
#include <exception>

#include "utils.hpp"
#include "StatusCode.hpp"
#include "Server.hpp"
#include "MultipartData.hpp"
#include "StatusCodeException.hpp"

# define URL_MAX_LEN 2083

class Request 
{
	public:
		typedef std::map<std::string, std::list<std::string>>::const_iterator	fields_it;
		typedef std::list<std::string>::const_iterator 							fields_values_it;
		typedef std::map<std::string, std::string>::const_iterator				post_mapit;
		typedef std::list<MultipartData *>::const_iterator						post_listit;

		// CONST/DESTR
		Request(const char *request, size_t size, Server *server);
		Request(const Request &instance);
		~Request();
		
		// GETTERS/SETTERS
		const char *get_request() const;
		std::string get_method() const;
		std::string get_target() const;
		std::string get_version() const;
		std::map<std::string, std::list<std::string>> get_fields() const;
		std::map<std::string, std::string> &get_defaultDatas();
		std::list<MultipartData *> &get_multipartDatas();

		class RequestException : public StatusCodeException {
			public:
				RequestException(const int code) : StatusCodeException(code) {}
		};

	private:
		Request();

		// PARSE ALL
		const char * request;
		size_t request_size;
		std::string_view _request;
		Server *_server;
		
		// PARSE HEADER
		std::string _method;
		std::string _target;
		std::map<std::string, std::list<std::string> > _fields;
		void _parse_start_line(std::string startLine);
		void _trim_sides(std::string & str);
		void _split_field(size_t separator, size_t lastchar);
		int _parse_header();

		// PARSE BODY
		const char *_body;
		size_t _body_len;
		// parse default type
		std::map<std::string, std::string> _postNameValue;
		void _parse_defaultDataType(std::string_view &formDatas);
		void _parse_body();
		// parse multipart type
		std::list<MultipartData *> _postMultipart;
		void _parse_multipartDataType(fields_it type);
		std::string _find_value_from_boundry_block(std::string_view &block, const char *strtofind, const char *strtolen, char stop);
		bool _check_filetype(std::string contentType);
		void _free_multipartDatas();

		// PRINT DATAS
		void _print_fields() const;
		void _print_defaultDatas() const;
		void _print_multipartDatas() const;
};

#endif
