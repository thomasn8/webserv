#include "../includes/Request.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string rawMessage) : _rawMessage(rawMessage) {
    std::string str;
    std::istringstream raw(this->_rawMessage);
    getline(raw, str);
    _check_alone_CR();
    _parse_start_line(str);
    _parse_header(raw);

    // check les headers
    // check le body
}

Request::Request(const Request& instance) {
    *this = instance;
}

Request::~Request() {

}

// --------- Fonctions ------------

std::string Request::get_message() const {
    return this->_rawMessage;
}

std::string Request::get_method() const {
    return this->_method;
}

std::string Request::get_target()const {
    return this->_target;
}

std::string Request::get_version() const {
    return this->_version;
}

// RFC 9112 2.2. Message Parsing 4 paragraph about CR
void Request::_check_alone_CR() {
    std::string::iterator it;
    for (it = this->_rawMessage.begin(); it != this->_rawMessage.end(); it++) {
        if (*it == '\r' && *(it + 1) != '\n')
            *it = ' ';
    }
}

//start-line parsing
void Request::_parse_start_line(std::string startLine) {
    std::string tmp = startLine;
    std::string token;
    size_t pos = 0;

    for(int i = 0; i < 3; i++) {
        pos = tmp.find(' ');
        if (i < 2 && pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
        if (i == 0) {
            this->_method = tmp.substr(0, pos);
            if (!(this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE"))
                throw MessageException(METHOD_NOT_ALLOWED);
        }
        else if (i == 1) {
            this->_target = tmp.substr(0, pos);
            //checker la target? (question)
        }
        else if (i == 2) {
            this->_version = tmp.substr(0, pos);
            if (this->_version != "HTTP/1.1")
                throw MessageException(HTTP_VERSION_UNSUPPORTED);
        }
        tmp.erase(0, pos + 1);
    }
    if (pos != std::string::npos)
        throw MessageException(BAD_REQUEST);
}

void Request::_parse_header(std::istringstream &raw) {
    std::string str;

    getline(raw, str);
    while (!str.empty() && str != "\r\n") {
        
        getline(raw, str);
    }

}

// --------- Operator overload ------------

Request &Request::operator=(const Request &instance) {
    this->_rawMessage = instance._rawMessage;
    this->_method = instance._method;
    this->_target = instance._target;
    this->_version = instance._version;

    this->_hasBody = instance._hasBody;
    this->_body = instance._body;

    std::map<std::string, std::string>::const_iterator it;
    std::map<std::string, std::string>::const_iterator it2;
    it2 = this->_fields.begin();
    for (it = instance._fields.begin(); it != instance._fields.end(); it++) {
        it2 = it;
        it2++;
    }
    return *this;
}
