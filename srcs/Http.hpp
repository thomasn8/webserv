/*
	Ébauche/idées de classes pour gérer notre data structure dans ce projet

	PARSING DES MESSAGES CLIENT-SERVER: header + content
*/

#ifndef HTTP_HPP
# define HTTP_HPP

#include <string>
#include <vector>
#include <utility>

#include "StatusCode.hpp"

// methods to implement
# define GET 1
# define POST 2
# define DELETE 3

/*
Exemples simplifiés de http message de type ...

HTTP request:
	GET /hello.html HTTP/1.1
	Host: 0.0.0.0
	Accept-Language: en, vi

HTTP response:
	HTTP/1.1 200 OK
	Server: Hello
	Content-Length: 13
	Content-Type: text/plain

	Hello, world
*/

class HttpMessage	// classe abstraite
{
	public:
		HttpMessage();
		virtual ~HttpMessage() = 0;

	protected:
		std::string _version;
		std::vector<std::pair<std::string, std::string> > _headersKeyVal;
		std::string _messageBody;
};

class HttpRequest : public HttpMessage
{
	public:
		HttpRequest();
		~HttpRequest();

	private:
		int _method;
		std::string _uri;
};

class HttpResponse : public HttpMessage
{
	public:
		HttpResponse();
		~HttpResponse();

	private:
		int _statusCode;
};

#endif
