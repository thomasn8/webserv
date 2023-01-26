#include "../includes/Response.hpp"

//TO DO Michele
// 1. default index et autoindex ++ rechecker les routes
// 2. GET avec des query? (?fav_language=CSS)
// 3. method POST
// 4. methos DELETE

// ---------Constructor and destructor ------------

// constructor for error response
Response::Response(const int code, Server *server, std::string * finalMessage) : _server(server), _version(std::string("HTTP/1.1")), _finalMessage(finalMessage) {
    char            *date;
    std::string     body;
	std::string 	codestr = std::to_string(code);
    
    _error_messages();
    date = Rfc1123_DateTimeNow();
    this->_header = this->_version + " " + codestr + " " + this->_errorMsg[code] + "\r\n" +
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
                <p>" + this->_errorMsg[code] + "</p> \
            </body> \
            </html>";

        // *this->_finalMessage = this->_header + "Content-Length: " + std::to_string(std::string(body).length()) + "\r\n\r\n" + body;
        *this->_finalMessage = this->_header + "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
        free(date);
    }
}

// constructor for normal response
Response::Response(Request *request, Server *server, std::string * finalMessage) : 
    _request(request), 
    _server(server),
	_finalMessage(finalMessage),
    _version(std::string("HTTP/1.1")),
    _isCGI(false),
    _targetFound(false) {
    if (request->get_method() == GET)
        _response_get();
    else if (request->get_method() == POST)
        _response_post();
    else if (request->get_method() == DELETE)
        _response_delete();
    else
        throw ResponseException(HTTP_VERSION_UNSUPPORTED);
}

void Response::_error_messages() {
    this->_errorMsg[400] = "BAD_REQUEST";
    this->_errorMsg[404] = "NOT_FOUND";
    this->_errorMsg[405] = "METHOD_NOT_ALLOWED";
    this->_errorMsg[500] = "INTERNAL_SERVER_ERROR";
    this->_errorMsg[505] = "HTTP_VERSION_UNSUPPORTED";
}

int Response::_check_error_pages(const int code) {
    std::list<std::pair<int, std::string>> &errorPages = this->_server->get_errorpages();
    std::list<std::pair<int, std::string>>::iterator  it;

    for (it = errorPages.begin(); it != errorPages.end(); it++) {
        if ((*it).first == code) {
            if (access( (*it).second.c_str(), F_OK ) != -1) {
                this->_path = (*it).second;
                return 1;
            }
        }
    }
    return 0;
}

void Response::_response_get() {
    char *date;

    _check_target_in_get(this->_request->get_target());
    date = Rfc1123_DateTimeNow();
    this->_header = this->_version + " 200 " + "OK\r\n" +
        "Content-Type: text/html, charset=utf-8\r\n" +
        "Server: pizzabrownie\r\n" +
        "Date: " + date + "\r\n";
    free(date);
    if (this->_isCGI == true)
        _make_CGI();
    else
        _make_response();
}

int Response::_make_CGI() {
    int         fd[2];
	pid_t       pid;
    int		    status;
    char        *cgi;

    cgi = (char *)malloc(sizeof(char) * this->_server->get_client_max_body_size());
    if (pipe(fd) == -1) {return -1;}
	pid = fork();
	if (pid == -1) {exit(EXIT_FAILURE);}
	if (pid == 0)
	{
        if (PRINT_CGI_GET)
            std:: cout << "path:" << this->_path << std::endl;
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {exit(EXIT_FAILURE);}
        execlp("php", "php", this->_path.c_str(), NULL);
        std::cerr << "Error launching cgi: " << this->_path << std::endl;
        write(1, "500", 3);
        exit(0);
	}
    else {
        close(fd[1]);
        if (read(fd[0], cgi, this->_server->get_client_max_body_size()) < 0) {
            close(fd[0]);
            return (1);
        }
        waitpid(pid, &status, 0);
        if (is_number(cgi))
            throw ResponseException(atoi(cgi));
        if (PRINT_CGI_GET)
            std:: cout << "cgi recieve:" << cgi << std::endl;
       *this->_finalMessage = this->_header + "Content-Length: " + std::to_string(std::string(cgi).length()) + "\r\n\r\n" + cgi;
        free(cgi);
        if (PRINT_HTTP_RESPONSE)
            std:: cout << *this->_finalMessage << std::endl;
        }
    return (0);
}

void Response::_make_response() {
    std::string body;

    std::ifstream f(this->_path);
    if(f) {
      std::ostringstream ss;
      ss << f.rdbuf();
      body = ss.str();
   }
    *this->_finalMessage = this->_header + "Content-Length: " + std::to_string(std::string(body).length()) + "\r\n\r\n" + body;
    if (PRINT_HTTP_RESPONSE)
        std:: cout << *this->_finalMessage << std::endl;
}

int Response::_check_redirections(std::string &target, std::deque<Location> &locations) {
    std::string                     redir = target;
    std::deque<Location>::iterator  it;
    std::list<Trio>::iterator       it2;

    if (PRINT_RESPONSE_PARSING) {
        std::cout << "________check redirections_____" << std::endl;
        std::cout << "Target before: " << target << std::endl;
    }
    for (it = locations.begin(); it != locations.end(); it++) {
        std::list<Trio> &trio = (*it).get_redirections();
        for (it2 = trio.begin(); it2 != trio.end(); it2++) {
            if (&target[1] == (*it2).first) {
                if (!((*it2).second.empty()))
                    redir =  (*it2).second;
                else
                    redir = std::to_string((*it2).third);
            }
        }
    }
    if (PRINT_RESPONSE_PARSING)
        std::cout << "Redir: " << redir << std::endl;
    if (is_number(redir))
        throw ResponseException(stoi(redir));
    if (target != redir) {
        target.erase(target.begin()+1, target.end());
        target.replace(1, redir.length(), redir);
        if (PRINT_RESPONSE_PARSING)
            std::cout << "Target after: " << target << std::endl;
        return 1;
    }
    if (PRINT_RESPONSE_PARSING)
            std::cout << "Target after: " << target << std::endl;
    return 0;
}

void Response::_check_locations(std::string &target, std::deque<Location> &locations) {
    std::string                     path;
    std::string                     root;
    std::deque<Location>::iterator  it;

    if (PRINT_RESPONSE_PARSING)
        std::cout << "________check locations_____" << std::endl;
    for (it = locations.begin(); it != locations.end(); it++) {
        root = (*it).get_root();
        path = root + target;
        if (PRINT_RESPONSE_PARSING) {
            std::cout << "location root: " << root << std::endl;
            std::cout << "target: " << target << std::endl;
            std::cout << "path: " << path << std::endl;
        }
        if (access( path.c_str(), F_OK ) != -1) {
            if (root.find("cgi_bin", root.length() - 7) != std::string::npos)
                this->_isCGI = true;
            this->_targetFound = true;
            this->_path = path;
            if (PRINT_RESPONSE_PARSING)
                std::cout << "path found: " << this->_path << std::endl;
        }
    }
}

void Response::_check_root(std::string &target) {
    std::string path;
    std::string root;

    if (PRINT_RESPONSE_PARSING)
        std::cout << "________check root_____" << std::endl;
    root = this->_server->get_root();
    path = root + target;
    if (PRINT_RESPONSE_PARSING)
        std::cout << "path: " << path << std::endl;
    if (access( path.c_str(), F_OK ) != -1) {
        if (root.find("cgi_bin", root.length() - 7) != std::string::npos)
            this->_isCGI = true;
        this->_targetFound = true;
        this->_path = path;
        if (PRINT_RESPONSE_PARSING)
            std::cout << "path found: " << this->_path << std::endl;
    }
}

void Response::_check_target_in_get(std::string target) {
    std::string                     redir = target;
    std::deque<Location>            &locations = this->_server->get_locations();

    if (*target.begin() != '/')
        throw ResponseException(BAD_REQUEST);
    while (_check_redirections(target, locations)) {};
    _check_locations(target, locations);
    if (this->_targetFound == false)
        _check_root(target);
    if (this->_targetFound == false)
        throw ResponseException(NOT_FOUND);
    if (PRINT_FINAL_TARGET)
        std::cout << "path found: " << this->_path << std::endl;
}

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

void Response::_response_delete() {

}

Response::Response(const Response& instance) : _request(instance._request), _server(instance._server) {
    *this = instance;
}

Response::~Response() {

}

// --------- Fonctions ------------

std::string Response::getMessage() const {
    return *this->_finalMessage;
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

Response &Response::operator=(const Response &instance) {
    this->_request = instance._request;
    this->_server = instance._server;
    this->_finalMessage = instance._finalMessage;
    this->_statusCode = instance._statusCode;
    this->_reason = instance._reason;
    this->_version = instance._version;

    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
