#include "../includes/Response.hpp"

// ---------Constructor and destructor ------------

// constructor for error response
Response::Response(const int code, Server *server, char **responseStr, size_t *responseSize) : 
_server(server), _version(std::string("HTTP/1.1")), _finalMessage(responseStr), _finalMessageSize(responseSize) {
    std::string     body;
	std::string 	codestr = std::to_string(code);
    std::string		date = Rfc1123_DateTimeNow();

    this->_header = "HTTP/1.1 " + std::to_string(code) + (" " +this->_status_messages(code)) + "\r\n" +
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
Response::Response(Request *request, Server *server, char **responseStr, size_t *responseSize) : 
    _request(request), 
    _server(server),
    _version(std::string("HTTP/1.1")),
	_finalMessage(responseStr),
	_finalMessageSize(responseSize),
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
    std::list<std::pair<int, std::string>> &errorPages = this->_server->get_errorpages();
    std::list<std::pair<int, std::string>>::iterator  it;

    for (it = errorPages.begin(); it != errorPages.end(); it++) {
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

void Response::_make_final_message(std::string &header, const char *body, std::filebuf *pbuf, size_t len) {
	size_t header_size = header.size();
	std::string bodysize = std::to_string(len);
	int bodysize_len = bodysize.size();

	// calc len + allocate
	*this->_finalMessageSize = header_size;
	*this->_finalMessageSize += 20; // for "Content-Length: \r\n\r\n"
	*this->_finalMessageSize += bodysize_len;
	*this->_finalMessageSize += len;
	*this->_finalMessage = (char *)malloc(*this->_finalMessageSize * sizeof(char));

	// set memory
	char *tmp = *this->_finalMessage;
	memcpy(tmp, header.c_str(), header_size);
	tmp += header_size;
	memcpy(tmp, "Content-Length: ", 16);
	tmp += 16;
	memcpy(tmp, bodysize.c_str(), bodysize_len);
	tmp += bodysize_len;
	memcpy(tmp, "\r\n\r\n", 4);
	tmp += 4;
	if (body)
		memcpy(tmp, body, len);
	else
		pbuf->sgetn(tmp, len);
}


void Response::_make_response() {
    std::ifstream ifs(this->_target, std::ifstream::binary);
	if (!ifs.is_open())
	    throw ResponseException(INTERNAL_SERVER_ERROR);												

	// get pointer to associated buffer object
	std::filebuf *pbuf = ifs.rdbuf();

	// get file size using buffer's members
	size_t size = pbuf->pubseekoff(0,ifs.end,ifs.in);
	pbuf->pubseekpos(0,ifs.in);
	if (size > this->_server->get_max_body_size())
		throw ResponseException(INTERNAL_SERVER_ERROR);										

	this->_make_final_message(this->_header, NULL, pbuf, size);
	ifs.close();
    if (PRINT_HTTP_RESPONSE)
        std:: cout << *this->_finalMessage << std::endl;
} 

// _______________________   GET   _____________________________ //

void Response::_response_get() {
    std::string date = Rfc1123_DateTimeNow();
    this->_header = this->_version + " " + this->_statusCode + " " + _status_messages(atoi(this->_statusCode.c_str())) + "\r\n" +
        "Content-Type: " + this->_targetType + "\r\n" +
        "Server: pizzabrownie\r\n" +
        "Date: " + date + "\r\n";
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
    if (PRINT_HTTP_RESPONSE)
        std:: cout << this->_finalMessage << std::endl;
}

// _______________________   POST   _____________________________ //

void Response::_decript_img() {
    
}

void Response::_response_post() {
    // std::string body = this->_request->get_body();
    // size_t pos = body.find("image/jpeg\r\n") + 14;
    // std::cout << "pos: " <<pos << std::endl;;
    // body.erase(0, pos);
    // pos = body.find("------");
    // body.erase(pos, body.length());
    // // std::cout << "body:" << body << std::endl;
    // // std::cout << "body length: " << body.length() << std::endl;
    // // std::cout << "content-length: " << *this->_request->get_fields()["Content-Length"].begin() << std::endl;

    // std::ofstream file("text.txt", std::ofstream::binary | std::ofstream::out);

    // char buffer[body.length()];
    // int bodySize = body.length();

    // file.write(body.c_str(), bodySize);
    // memset(buffer, 0, body.length());
    // file.close();
}


// _______________________   DELET   _____________________________ //

void Response::_response_delete() {

}


// _______________________   CGI   _____________________________ //

int Response::_make_CGI() {
    int         fd[2];
	pid_t       pid;
    int		    status;
    char        *cgi;
	size_t		cgi_size;
    std::string cgiType;

    cgiType = _what_kind_of_cgi(this->_target);
	cgi_size = this->_server->get_max_body_size();
    cgi = (char *)malloc(sizeof(char) * cgi_size);
    if (pipe(fd) == -1) {return -1;}
	pid = fork();
	if (pid == -1) {exit(EXIT_FAILURE);}
	if (pid == 0)
	{
        if (PRINT_CGI_GET)
            std:: cout << "path:" << this->_target << std::endl;
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {exit(EXIT_FAILURE);}
        execlp(cgiType.c_str(), cgiType.c_str(), this->_target.c_str(), NULL);
        std::cerr << "Error launching cgi: " << this->_target << std::endl;
        write(1, "500", 3);
        exit(0);
	}
    else {
        close(fd[1]);
        if (read(fd[0], cgi, cgi_size) < 0) {
            close(fd[0]);
            return (1);
        }
        waitpid(pid, &status, 0);
        if (is_number(cgi))
            throw ResponseException(atoi(cgi));
        if (PRINT_CGI_GET)
            std:: cout << "cgi recieve:" << std::string(cgi, cgi_size) << std::endl;

		this->_make_final_message(this->_header, cgi, NULL, cgi_size);
		free(cgi);
        if (PRINT_HTTP_RESPONSE)
            std:: cout << std::string(*this->_finalMessage, *this->_finalMessageSize) << std::endl;
        }

    return (0);
}

// _______________________   ROUTES   _____________________________ //

// loop through Locations container and change target if there is redirections
int Response::_check_redirections(std::string &target, 
        std::deque<Location> &locations, std::deque<Location>::iterator &locationFound) {
    std::deque<Location>::iterator  it;
    std::list<Trio>::iterator       it2;

    for (it = locations.begin(); it != locations.end(); it++) {
        std::list<Trio> &trio = (*it).get_redirections();
        for (it2 = trio.begin(); it2 != trio.end(); it2++) {
            if (target.compare((*it2).first) == 0) {
                if (!((*it2).second.empty())) {
                    target = (*it2).second;
                    this->_statusCode = std::to_string((*it2).third);
                }
                else {
                    target = std::to_string((*it2).third);
                    if (is_number(target))
                        throw ResponseException(stoi(target));
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
        std::deque<Location> &locations, std::deque<Location>::iterator &locationFound) {
    std::deque<Location>::iterator  it;
    int                             len;
    std::string                     tmp;
    
    tmp = target;
    for (it = locations.begin(); it != locations.end(); it++) {
        len = (*it).get_route().length();
        if (!(*(*it).get_route().begin() == '/')) {
            if (tmp.compare(tmp.length() - len, tmp.length(),(*it).get_route()) == 0) {
                tmp = (*it).get_root() + tmp;
                while (_check_redirections(tmp, locations, locationFound)) {};
                if (access(tmp.c_str(), F_OK) != -1) {
                    target = tmp;
                    this->_cgi = target;
                    return 1;
                }
            }
        }
    }
    target = this->_server->get_root() + target;
    return 0;
}

// check if there is a default index file
int Response::_is_index_file(std::string &target, std::list<std::string> indexes) {
    std::list<std::string>::iterator      it;
    
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
        std::deque<Location> &locations, std::deque<Location>::iterator &locationFound) {
    std::deque<Location>::iterator  it;

    for (it = locations.begin(); it != locations.end(); it++) {
        if (target.compare((*it).get_root()) == 0) {
            this->_targetFound = true;
            locationFound = it;
        }
    }
}

// check if there is a Location corresponding to the target as a file
void Response::_check_locations(std::string &target, 
        std::deque<Location> &locations, std::deque<Location>::iterator &locationFound) {
    std::deque<Location>::iterator  it;
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
void Response::_check_methods_in_location(std::deque<Location>::iterator &locationFound) {
    std::list<std::string>::iterator  it;
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

    // std::cout << target << std::endl;
    if (pos != 0) {
        if (target.compare(pos, 3, "css") == 0) 
            return "text/css, charset=utf-8";
        else if (target.compare(pos, 2, "js") == 0)
            return "text/javascript, charset=utf-8";
        else if (target.compare(pos, 4, "html") == 0)
            return "text/html, charset=utf-8";
        else if (target.compare(pos, 3, "svg") == 0)
            return "image/svg+xml, charset=utf-8";
        else if (target.compare(pos, 4, "woff") == 0)
            return "font/woff";
        else if (target.compare(pos, 4, "woff") == 0)
            return "font/woff";
        else if (target.compare(pos, 3, "ttf") == 0)
            return "font/ttf";
        else if (target.compare(pos, 3, "otf") == 0)
            return "font/otf";
        else if (target.compare(pos, 3, "jpg") == 0)
            return "image/jpeg";
        else if (target.compare(pos, 3, "png") == 0)
            return "image/png";
    }
    return "text/html, charset=utf-8";
}

// Main function to make de routes
void Response::_check_target() {
    std::deque<Location>            &locations = this->_server->get_locations();
    std::deque<Location>::iterator  locationFound;

    this->_target = this->_request->get_target();
    if (PRINT_RECIEVED_TARGET)
        // std::cout << "Target at begin: " << this->_target << std::endl;
    if (*this->_target.begin() != '/')
        throw  ResponseException(BAD_REQUEST);
    if (this->_target.find('.') == std::string::npos) { // if it's a directory
        while (this->_target.back() == '/')
            this->_target.pop_back();
        this->_target = this->_server->get_root() + this->_target;
        while (_check_redirections(this->_target, locations, locationFound)) {};
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
            _check_methods_in_location(locationFound);
            this->_uploadsDir = locationFound->get_uploadsdir();
            if (!locationFound->get_contentTypes().empty())
                this->_contentType = locationFound->get_contentTypes();
            // if (!locationFound->get_cgi().empty())
            //     this->_cgi = this->_target;
        }
        else {
            if (access( this->_target.c_str(), F_OK ) != -1) {
                this->_targetFound = true;
                // if (!_what_kind_of_cgi(this->_target).empty())
                //     this->_cgi = this->_target;
            }
            else
                throw  ResponseException(NOT_FOUND);
        }
    }
    this->_targetType = _what_kind_of_extention(this->_target);
    if (this->_statusCode.empty())
        this->_statusCode = std::to_string(HTTP_OK);
    if (PRINT_FINAL_TARGET) {
        std::cout << "final target: " << this->_target << std::endl;
        std::cout << "autoindex: " << this->_autoindex << std::endl;
        std::cout << "cgi: " << this->_cgi << std::endl;
        std::cout << "upload dir: " << this->_uploadsDir << std::endl;
        std::cout << "status code: " << this->_statusCode << std::endl;
        std::cout << "content types: ";
        std::list<std::string>::iterator  it;
        if (!this->_contentType.empty()) {
            for (it = this->_contentType.begin(); it != this->_contentType.end(); it++) {
                std::cout << *it << " \n";
            }
        }
    }
}

// --------- Fonctions getteur ------------

char * Response::getFinaleMessage() const {
    return *this->_finalMessage;
}

size_t Response::getFinaleMessageSize() const {
    return *this->_finalMessageSize;
}

std::string Response::getStatusCode() const {
    return this->_statusCode;
}

std::string Response::getReason() const {
    return this->_reason;
;
}

std::string Response::getVersion() const {
    return this->_version;
}

// --------- Operator overload ------------

Response &Response::operator=(const Response &instance) { 						// PAS A JOUR, copie pas toutes les variables
    this->_request = instance._request;
    this->_server = instance._server;
    this->_finalMessage = instance._finalMessage;
    this->_finalMessageSize = instance._finalMessageSize;
    this->_statusCode = instance._statusCode;
    this->_reason = instance._reason;
    this->_version = instance._version;
    return *this;
}
