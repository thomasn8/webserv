#include "../includes/Response.hpp"

// ---------Constructor and destructor ------------

// constructor for error response
Response::Response(const int code, Server *server, struct responseInfos *res) : 
_server(server), _response(res) {
    std::string     body;
	std::string 	codestr;
	std::ostringstream convert;
    std::string		date;

	convert << code;
	codestr = convert.str();
    Rfc1123_DateTimeNow(date);
    this->_header = "HTTP/1.1 " + codestr + (" " +this->_status_messages(code)) + "\r\n" +
		"Content-Type: text/html, charset=utf-8\r\n" +
		"Server: pizzabrownie\r\n" +
		"Date: " + date + "\r\n";

    if (_check_error_pages(code)) {
        _make_response();
    } else {
        body = "<!DOCTYPE html> \
            <html lang=\"en\"> \
            <head> \
                <meta charset=\"UTF-8\"> \
                <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> \
                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> \
                <title>" + codestr + "</title> \
            </head> \
            <body> \
                <h1>Error " + codestr + "</h1> \
                <p>" + this->_status_messages(code) + "</p> \
            </body> \
            </html>";
		this->_make_final_message(this->_header, body.c_str(), NULL, body.size());
    }
}

// constructor for normal response
Response::Response(Request *request, Server *server, struct responseInfos *res) : 
    _request(request), 
    _server(server),
	_response(res),
    _autoindex(false),
    _targetFound(false) {
    _check_target();
    if (request->get_method() == GET)
        _response_get();
    else if (request->get_method() == POST)
        _response_post();
    else if (request->get_method() == DELETE)
        _response_delete();
    else
        throw ResponseException(HTTP_VERSION_UNSUPPORTED);
}

Response::Response(const Response& instance) : _request(instance._request), _server(instance._server) {
    *this = instance;
}

Response::~Response() {}

// _______________________   Status code and errors   _____________________________ //
std::string Response::_status_messages(int code) {
	switch (code)
	{
		case 200:
			return "OK";
		case 400:
			return "BAD REQUEST";
		case 403:
			return "FORBIDDEN";
		case 404:
			return "NOT FOUND";
		case 405:
			return "METHOD NOT ALLOWED";
		case 408:
			return "REQUEST TIMEOUT";
		case 413:
			return "PAYLOAD TOO LARGE";
		case 415:
			return "MEDIA UNSUPPORTED";
		case 431:
			return "HEADERS TOO LARGE";
		case 500:
			return "INTERNAL SERVER ERROR";
		case 505:
			return "HTTP VERSION UNSUPPORTED";
		default:
			return "NO CORRESPONDING ERROR MESSAGE";
	}
}

// check if there is an error pages set in conf file
int Response::_check_error_pages(const int code) {
    std::list<std::pair<int, std::string> >::const_iterator  it;

    for (it = this->_server->get_errorpages().begin(); it != this->_server->get_errorpages().end(); it++) {
        if ((*it).first == code) {
            if (access( (*it).second.c_str(), F_OK ) != -1) {
                this->_target = (*it).second;
                return 1;
            }
        }
    }
    return 0;
}

// _______________________   Final Response Creation   _____________________________ //

void Response::_make_final_message(std::string const &header, const char *body, std::filebuf *pbuf, size_t len) {
	std::ostringstream convert;
	convert << len;
	std::string lenStr = convert.str();

	_response->header = header + "Content-Length: " + lenStr + "\r\n\r\n";
	_response->body = (char *)malloc(len * sizeof(char));
	_response->body_size = len;
	if (body)
		memcpy(_response->body, body, len);
	else
		pbuf->sgetn(_response->body, len);
}


void Response::_make_response() {
    std::ifstream ifs(this->_target.c_str(), std::ifstream::binary);
	if (!ifs.is_open())
	    throw ResponseException(INTERNAL_SERVER_ERROR);												

	std::filebuf *pbuf = ifs.rdbuf();
	size_t size = pbuf->pubseekoff(0,ifs.end,ifs.in);
	pbuf->pubseekpos(0,ifs.in);
	if (size > this->_server->get_max_body_size())
		throw ResponseException(INTERNAL_SERVER_ERROR);										

	this->_make_final_message(this->_header, NULL, pbuf, size);
	ifs.close();
} 

// _______________________   GET   _____________________________ //

void Response::_response_get() {
    std::string		date;

    Rfc1123_DateTimeNow(date);
    this->_header = "HTTP/1.1 " + this->_statusCode + " " + _status_messages(atoi(this->_statusCode.c_str())) + "\r\n" +
        "Content-Type: " + this->_targetType + "\r\n" +
        "Server: pizzabrownie\r\n" +
        "Date: " + date + "\r\n";
    _check_body();
    if (!this->_cgi.empty())
        _make_CGI();
    else if (this->_autoindex)
        _make_autoindex();
    else
        _make_response();
}

void Response::_make_autoindex() {
    std::string     body;
    std::string     liste;
    DIR             *dir;
    struct dirent   *ent;

    if ((dir = opendir (this->_target.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0)
                liste = liste + "<li style=\"list-style-type: none;\"><a href=\"" + this->_request->get_target() + "/" + ent->d_name + "\">" + ent->d_name + "</a></li>\n";
        }
        closedir (dir);
    }
    body = "<!DOCTYPE html> \
            <html lang=\"fr\"> \
            <head> \
                <meta charset=\"UTF-8\"> \
                <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> \
                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> \
                <title>Index</title> \
            </head> \
            <body> \
                <h1>Index of " + this->_request->get_target() + "</h1> \
                <ul>" + liste + "</ul>\
            </body> \
            </html>";
    this->_make_final_message(this->_header, body.c_str(), NULL, body.size());
}

// _______________________   POST   _____________________________ //

//checker les files extansions
void Response::_upload_file(MultipartData *data) {
    //check if content type is accepted in location
    bool ok = false;
    int pos = 0;
    pos = data->get_contentType().find("/") + 1;
    std::list<std::string>::iterator  it;
    if (!this->_contentType.empty()) {
        for (it = this->_contentType.begin(); it != this->_contentType.end(); it++) {
            if (data->get_contentType().compare(pos, (*it).length(), (*it)) == 0)
                ok = true;
        }
    }
    if (!ok)
        throw ResponseException(MEDIA_UNSUPPORTED);

    // replace spaces
    std::string fileName = data->get_fileName();
    pos = fileName.find(" ");
    while (pos != -1)
    {
        fileName.replace(pos, 1, "%20");
        pos = fileName.find(" ");
    }

    //check if directory exist
	if (access(this->_uploadsDir.c_str(), F_OK | R_OK | W_OK ) == -1)
        throw ResponseException(INTERNAL_SERVER_ERROR);

    //upload file
	std::string f = this->_uploadsDir + std::string("/") + fileName;
    std::ofstream file(f.c_str(), std::ofstream::binary | std::ofstream::out);
    char buffer[data->get_valueLen()];
    int bodySize = data->get_valueLen();
    file.write(data->get_value(), bodySize);
    memset(buffer, 0, data->get_valueLen());
    file.close();
}

//check if there is file to upload in body or prepare body for cgi
void Response::_check_body() {
	if (this->_request->get_postDefault().empty() == false)
		this->_body = this->_request->get_postDefault();
	else if (this->_request->get_multipartDatas().empty() == false)
	{
		std::list<MultipartData *> const &datas = this->_request->get_multipartDatas();
        for (Request::mutlipart_it it = datas.begin(); it != datas.end(); it++) {
            if ((*it)->get_file()) {
                _upload_file((*it));
                this->_body += (*it)->get_name() + "=" + (*it)->get_fileName() + "&";
            }
            else
                this->_body += (*it)->get_name() + "=" + (*it)->get_value() + "&";
        }
        if (!this->_body.empty() && this->_body.compare(this->_body.length() - 1, 1, "&") == 0)
            this->_body.erase(_body.size() - 1, 1);
	}
}

void Response::_response_post() {
    std::string		date;

    Rfc1123_DateTimeNow(date);
    this->_header = "HTTP/1.1 " + this->_statusCode + " " + _status_messages(atoi(this->_statusCode.c_str())) + "\r\n" +
        "Content-Type: " + this->_targetType + "\r\n" +
        "Server: pizzabrownie\r\n" +
        "Date: " + date + "\r\n";
    _check_body();
    if (!this->_cgi.empty())
        _make_CGI();
    else if (this->_autoindex)
        _make_autoindex();
    else
        _make_response();
}

// _______________________   DELET   _____________________________ //

void Response::_response_delete() {  
    if (remove(this->_target.c_str()))
        throw ResponseException(FORBIDDEN);
    std::string		date;

    Rfc1123_DateTimeNow(date);
    this->_header = "HTTP/1.1 " + this->_statusCode + " " + _status_messages(atoi(this->_statusCode.c_str())) + "\r\n" +
        "Content-Type: text/html" "\r\n" +
        "Server: pizzabrownie\r\n" +
        "Date: " + date + "\r\n";
    std::string body = "<!DOCTYPE html> \
            <html lang=\"fr\"> \
            <head> \
                <meta charset=\"UTF-8\"> \
                <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> \
                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> \
                <title>Index</title> \
            </head> \
            <body> \
                <h1>File deleted successfully</h1> \
            </body> \
            </html>";
    this->_make_final_message(this->_header, body.c_str(), NULL, body.size());
}

// _______________________   CGI   _____________________________ //

char **Response::_prepare_env() {
    std::map<std::string, std::list<std::string> >          fields = this->_request->get_fields();
    std::map<std::string, std::string>::const_iterator      it;
    std::list<std::string>::iterator                        it2;

    std::string     query("");
    std::string     accept("");
    std::string     accepLang("");
    std::string     userAgent("");
    std::string     host(*(fields["Host"]).begin());
    std::string     contentType;
    std::string     contentLengh;
    std::string     cookies(*(fields["Cookie"]).begin());
    char            **tmp;
    int             i = 0;
    int             j = 0;

    while (this->_server->get_env()[i] != NULL)
        i++;
    tmp = (char **)malloc(sizeof(char *) * (i + 14));
    i = 0;
    while (this->_server->get_env()[i] != NULL) {
        tmp[i] = strdup(this->_server->get_env()[i]);
        i++;
    }
    if (!(fields["Content-Type"].empty()))
       contentType = "application/x-www-form-urlencoded";
    if (!this->_body.empty())
	{
		std::ostringstream convert;
		convert << this->_body.length();
		contentLengh = convert.str();
	}
    else if (!(fields["Content-Length"].empty()))
        contentLengh = *(fields["Content-Length"]).begin();
    else
        contentLengh = "0";
    for (it2 = (fields["Accept"]).begin(); it2 != (fields["Accept"]).end(); it2++) {
        accept = accept.append(*it2);
        if (*it2 != *(fields["Accept"]).rbegin())
            accept.append(", ");
    };
    for (it2 = (fields["Accept-Language"]).begin(); it2 != (fields["Accept-Language"]).end(); it2++) {
        accepLang = accepLang.append(*it2);
        if (*it2 != *(fields["Accept-Language"]).rbegin())
            accepLang.append(", ");
    };
    for (it2 = (fields["User-Agent"]).begin(); it2 != (fields["User-Agent"]).end(); it2++) {
        userAgent = userAgent.append(*it2);
        if (*it2 != *(fields["User-Agent"]).rbegin())
            userAgent.append(", ");
    };

    while (j < 13) {
        if (j == 0)
            tmp[i] = strdup((std::string("SERVER_NAME=") + host).c_str());
        else if (j == 1)
            tmp[i] = strdup("SERVER_PROTOCOL=HTTP/1.1");
        else if (j == 2)
            tmp[i] = strdup((std::string("SERVER_PORT=") + this->_server->get_port_str()).c_str());
        else if (j == 3)
            tmp[i] = strdup((std::string("REQUEST_METHOD=") + this->_request->get_method()).c_str());
        else if (j == 4)
            tmp[i] = strdup((std::string("SCRIPT_NAME=") + this->_request->get_target()).c_str());
        else if (j == 5)
            tmp[i] = strdup((std::string("QUERY_STRING=") + this->_request->get_queryString()).c_str());
        else if (j == 6)
            tmp[i] = strdup("GATEWAY_INTERFACE=CGI/1.1");
        else if (j == 7) 
            tmp[i] = strdup((std::string("CONTENT_TYPE=") + contentType).c_str());
        else if (j == 8)
            tmp[i] = strdup((std::string("CONTENT_LENGTH=") + contentLengh).c_str());
        else if (j == 9)
            tmp[i] = strdup((std::string("HTTP_ACCEPT=") + accept).c_str());
        else if (j == 10)
            tmp[i] = strdup((std::string("HTTP_ACCEPT_LANGUAGE=") + accepLang).c_str());
        else if (j == 11)
            tmp[i] = strdup((std::string("HTTP_USER_AGENT=") + userAgent).c_str());
        else if (j == 12)
            tmp[i] = strdup((std::string("HTTP_COOKIE=") + cookies).c_str());
        i++;
        j++;
    }
    tmp[i] = NULL;
    return (tmp);
}

void Response::_execute_cgi() {
    std::string     cgiType;
    std::string     path;
    std::string     pathEnv;
    int             pos = 0;
    char            **tmpEnv;

    tmpEnv = _prepare_env();
    pathEnv = std::string(getenv("PATH"));
    cgiType = _what_kind_of_cgi(this->_target);
    pos = pathEnv.find(":");
    while (pos != -1) {
        path = pathEnv.substr(0, pos) + "/" + cgiType;
        execle(path.c_str(), path.c_str(), this->_target.c_str(), NULL, tmpEnv);
        pathEnv.erase(0, pos + 1);
        pos = pathEnv.find(":");
    }
    if (tmpEnv != NULL) {
        for(int i = 0; tmpEnv[i] != NULL; i++)
            free(tmpEnv[i]);
        free(tmpEnv);
        tmpEnv = NULL;
    }
}

int Response::_make_CGI() {
    int         fd[2];
    int         in[2];
	pid_t       pid;
    int		    status;
    char        *cgi;
    size_t		max_size;
    size_t		len;
    size_t      size = 0;

    
    if (pipe(fd) == -1) {return -1;}
    if (pipe(in) == -1) {return -1;}
	if (!this->_body.empty())
		write(in[1], this->_body.c_str(), this->_body.length());
	close(in[1]);
	pid = fork();
	if (pid == -1) {exit(EXIT_FAILURE);}
	if (pid == 0)
	{
        close(fd[0]);
        close(in[1]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {exit(EXIT_FAILURE);}
        if (dup2(in[0], STDIN_FILENO) == -1) {exit(EXIT_FAILURE);}
        _execute_cgi();
        std::cerr << "Error launching cgi: " << this->_target << std::endl;
        write(1, "500", 3);
        exit(0);
	}
    else {
        max_size = this->_server->get_max_body_size(); 
        cgi = (char *)malloc(sizeof(char) * max_size);
        std::string finalCgi;
        close(fd[1]);
        waitpid(pid, &status, 0);
        len = read(fd[0], cgi, max_size);
        while ( len > 0) {
            finalCgi.append(cgi, len);
            free(cgi);
            cgi = NULL;
            len = read(fd[0], cgi, max_size);
        }
        if (cgi != NULL) {
            free(cgi);
            cgi = NULL;
        }
        close(fd[0]);
        if (!finalCgi.empty())
            size = finalCgi.size();
        if (size > max_size || is_number(finalCgi))
            throw ResponseException(INTERNAL_SERVER_ERROR);
		this->_make_final_message(this->_header, finalCgi.c_str(), NULL, size);
    }
    return (0);
}

// _______________________   ROUTES   _____________________________ //

// loop through Locations container and change target if there is redirections
int Response::_check_redirections(std::string &target, std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound) {
    std::deque<Location>::const_iterator  it;
    std::list<Trio>::const_iterator       it2;

    for (it = locations.begin(); it != locations.end(); it++) {
        std::list<Trio> const &trio = (*it).get_redirections();
        for (it2 = trio.begin(); it2 != trio.end(); it2++) {
            if (target.compare((*it2).first) == 0) {
                if (!((*it2).second.empty())) {
                    target = (*it2).second;
					std::ostringstream convert;
					convert << (*it2).third;
					this->_statusCode = convert.str();
                }
                else {
					std::ostringstream convert;
					convert << (*it2).third;
					target = convert.str();
                    if (is_number(target))
                        throw ResponseException(atoi(target.c_str()));
                }
                this->_targetFound = true;
                locationFound = it;
                return 1;
            }
        }
    }
    return 0;
}

// add the good root before the target when it's a cgi
// for exemple for /images/medias.php
// it's a CGI and must go to www/cgi_bin/media/images/medias.php
// instead of html/media/images/medias.php
// For js it check if its a cgi and if not it's maybe a js file for html
int Response::_add_root_if_cgi(std::string &target, 
        std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound) {
    std::deque<Location>::const_iterator	it;
    int                           			len;
    std::string                   			tmp;
    std::string                   			stock;
    
    tmp = target;
    for (it = locations.begin(); it != locations.end(); it++) {
        len = (*it).get_route().length();
        if (!(*(*it).get_route().begin() == '/')) {
            if (tmp.compare(tmp.length() - len, tmp.length(),(*it).get_route()) == 0) {
                tmp = (*it).get_root() + tmp;
                while (_check_redirections(tmp, locations, locationFound)) {};
                if (access(tmp.c_str(), F_OK) != -1) {
                    target = tmp;
                    this->_targetFound = true;
                    locationFound = it;
                    if (!_what_kind_of_cgi(target).empty())
                        this->_cgi = target;
                    return 1;
                }
            }
        }
        else {
            if (target.find((*it).get_route()) == 0) { 
                tmp = target;
                if ((*it).get_route().find('.') != std::string::npos) {
                   size_t pos = (*it).get_route().find_last_of("/");
                   tmp.erase(0, pos);
                }
                else
                    tmp.erase(0, len - 1);
                tmp = (*it).get_root() + tmp;
                while (_check_redirections(tmp, locations, locationFound)) {};
                if (access(tmp.c_str(), F_OK) != -1) {
                    if (stock.empty() || tmp.size() > stock.size()) {
                        stock = tmp;
                        locationFound = it;
                    }
                }
            }
        }
    }
    if (!stock.empty()) {
        target = stock;
        this->_targetFound = true;
        if (!_what_kind_of_cgi(target).empty())
            this->_cgi = target;
        return 1;
    }
    target = this->_server->get_root() + target;
    return 0;
}

int Response::_add_root_dir(std::string &target, 
        std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound) {
    std::deque<Location>::const_iterator	it;
    int                           			len;
    std::string                   			tmp;
    std::string                   			stock;
    
    tmp = target;
    for (it = locations.begin(); it != locations.end(); it++) {
        len = (*it).get_route().length() - 1;
        if ((*(*it).get_route().begin() == '/')) {
            if (target.find((*it).get_route()) == 0) {
                tmp.erase(0, len);
                tmp = (*it).get_root() + tmp;
                while (_check_redirections(tmp, locations, locationFound)) {};
                if (access(tmp.c_str(), F_OK) != -1) {
                    if (stock.empty() || tmp.size() > stock.size()) {
                        stock = tmp;
                        locationFound = it;
                    }
                }
            }
        }
    }
    if (!stock.empty()) {
        target = stock;
        this->_targetFound = true;
        if (!_what_kind_of_cgi(target).empty())
            this->_cgi = target;
        return 1;
    }
    target = this->_server->get_root() + target;
    return 0;
}

// check if there is a default index file
int Response::_is_index_file(std::string &target, std::list<std::string> const &indexes) {
    std::list<std::string>::const_iterator      it;
    
    for (it = indexes.begin(); it != indexes.end(); it++) {
        if (access((target + "/" + *it).c_str(), F_OK) != -1) {
            target = target + "/" + *it;
            if (!_what_kind_of_cgi(this->_target).empty())
                    this->_cgi = target;
            return 1;
        }
    }
    return 0;
}

// check if there is a Location corresponding to the target as a directory
void Response::_check_locations_directory(std::string &target, 
        std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound) {
    std::deque<Location>::const_iterator  it;

    for (it = locations.begin(); it != locations.end(); it++) {
        // std::cout << "here: " << (*it).get_root() << std::endl;
        if (target.compare((*it).get_root()) == 0) {
            this->_targetFound = true;
            locationFound = it;
        }
    }
}

// check if there is a Location corresponding to the target as a file
void Response::_check_locations(std::string &target, 
        std::deque<Location> const &locations, std::deque<Location>::const_iterator &locationFound) {
    std::deque<Location>::const_iterator  it;
    size_t pos = 0;

    pos = target.find_last_of("/");
    for (it = locations.begin(); it != locations.end(); it++) {
        if (target.compare(0, pos, (*it).get_root()) == 0) {
            this->_targetFound = true;
            locationFound = it;
        }
    }
}

// check if the Method used is allowed in a Location
void Response::_check_methods_in_location(std::deque<Location>::const_iterator &locationFound) {
    std::list<std::string>::const_iterator  it;
    int i = 0;

    for (it = locationFound->get_methods().begin(); it != locationFound->get_methods().end(); it++) {
        if (*it == this->_request->get_method())
            i = 1;
    }
    if (i == 0)
        throw  ResponseException(METHOD_NOT_ALLOWED);
}

// return the cgi type
std::string Response::_what_kind_of_cgi(std::string &target) {
    if (target.find(".php", target.length() - 4) != std::string::npos)
        return "php";
    else if (target.find(".js", target.length() - 3) != std::string::npos)
        return "node";
    return "";
}

// return the file extention type
std::string Response::_what_kind_of_extention(std::string &target) {
    int pos = target.find_last_of(".") + 1;
	
	if (pos == 0)
		return "text/html, charset=utf-8";
	std::string ext = target.substr(pos);
	const std::string exts[] = {"css", "js", "html", "svg", "woff", "ttf", "otf", "jpg", "jpeg", "png"};
	const std::string corresponding[] = {
		"text/css, charset=utf-8",
		"text/javascript, charset=utf-8",
		"text/html, charset=utf-8",
		"image/svg+xml, charset=utf-8",
		"font/woff",
		"font/ttf",
		"font/otf",
		"image/jpeg",
		"image/jpeg",
		"image/png"
	};
	for (int i = 0; i < 10; i++)
	{
		if (ext == exts[i])
			return corresponding[i];
	}
	return "text/html, charset=utf-8";
}

// Main function to make de routes
void Response::_check_target() {
    std::deque<Location> &locations = this->_server->get_locations();
    std::deque<Location>::const_iterator  locationFound;

    this->_target = this->_request->get_target();
    // std::cout << "target at begin:" << this->_target << std::endl;
    if (*this->_target.begin() != '/')
        throw  ResponseException(BAD_REQUEST);
    if (this->_target.find('.') == std::string::npos) { // if it's a directory
        while (*(this->_target.rbegin()) == '/')
            this->_target.erase(this->_target.size() - 1, 1);
        if (!_add_root_dir(this->_target, locations, locationFound)) {
            while (_check_redirections(this->_target, locations, locationFound)) {};
            if (!this->_targetFound)
                _check_locations_directory(this->_target, locations, locationFound);
        } 
        if (!this->_targetFound)
            _check_locations_directory(this->_target, locations, locationFound);
        if (this->_targetFound) {
            _check_methods_in_location(locationFound);
            if (!_is_index_file(this->_target, locationFound->get_indexes())) {
                if (locationFound->get_autoindex())
                    this->_autoindex = true;
                else
                    throw  ResponseException(FORBIDDEN);
            }
            this->_uploadsDir = locationFound->get_uploadsdir();
            if (!locationFound->get_contentTypes().empty())
                this->_contentType = locationFound->get_contentTypes();
        }
        else {
            if (this->_target.compare(this->_server->get_root()) == 0) {
                if (!_is_index_file(this->_target, this->_server->get_indexes()))
                    throw  ResponseException(FORBIDDEN);
                if (this->_request->get_method().compare("DELETE") == 0)
                    throw  ResponseException(METHOD_NOT_ALLOWED);
                this->_uploadsDir = this->_server->get_root();
            }
            else
                throw  ResponseException(NOT_FOUND);
        }
    }
    else { // else it's a file
        if (!_add_root_if_cgi(this->_target, locations, locationFound)) {
            while (_check_redirections(this->_target, locations, locationFound)) {};
            if (!this->_targetFound)
                _check_locations(this->_target, locations, locationFound);
        }  
        if (this->_targetFound) {
            if (access(this->_target.c_str(), F_OK) == -1)
                throw  ResponseException(NOT_FOUND);
            this->_uploadsDir = locationFound->get_uploadsdir();
            _check_methods_in_location(locationFound);
            if (!locationFound->get_contentTypes().empty())
                this->_contentType = locationFound->get_contentTypes();
        }
        else {
            if (this->_request->get_method().compare("DELETE") == 0)
                throw  ResponseException(METHOD_NOT_ALLOWED);
            if (access( this->_target.c_str(), F_OK ) != -1) {
                this->_targetFound = true;
                this->_uploadsDir = this->_server->get_root();
            }
            else 
                throw  ResponseException(NOT_FOUND);
        }
    }
    // std::cout << "final target: " << this->_target << std::endl;
    // std::cout << "autoindex: " << this->_autoindex << std::endl;
    // std::cout << "cgi: " << this->_cgi << std::endl;
    // std::cout << "upload dir: " << this->_uploadsDir << std::endl;
    // std::cout << "status code: " << this->_statusCode << std::endl;
    // std::cout << "content types: ";
    // std::list<std::string>::iterator  it;
    // if (!this->_contentType.empty()) {
    //     for (it = this->_contentType.begin(); it != this->_contentType.end(); it++) {
    //         std::cout << *it << " \n";
    //     }
    // }
    this->_targetType = _what_kind_of_extention(this->_target);
    if (this->_statusCode.empty())
        this->_statusCode = "200";
}

// --------- Fonctions getteur ------------

std::string Response::getStatusCode() const {
    return this->_statusCode;
}

// --------- Operator overload ------------

Response &Response::operator=(const Response &instance) { 						
    this->_request = instance._request;
    this->_server = instance._server;
    this->_response = instance._response;
    this->_header = instance._header;
    this->_statusCode = instance._statusCode;
    this->_target = instance._target;
    this->_targetType = instance._targetType;
    this->_uploadsDir = instance._uploadsDir;
    this->_cgi = instance._cgi;
    this->_body = instance._body;
    this->_contentLength = instance._contentLength;
    this->_targetFound = instance._targetFound;

    return *this;
}
