#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string rawMessage) : _rawMessage(rawMessage) {
    std::string str;
    std::istringstream raw(this->_rawMessage);
    getline(raw, str);
    _check_alone_CR();
    _parse_start_line(str);
    _parse_header(raw);
    _parse_body(raw);
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

    tmp.erase(remove(tmp.begin(), tmp.end(), '\r'), tmp.end());
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
            if (this->_version.compare("HTTP/1.1") != 0) {
                throw MessageException(HTTP_VERSION_UNSUPPORTED);
            }
        }
        tmp.erase(0, pos + 1);
    }
    if (pos != std::string::npos)
        throw MessageException(BAD_REQUEST);
}

void Request::_split_field(size_t pos, std::string line) {
    std::string             key;
    std::string             tmp;
    std::list<std::string>  lstTmp;
    std::string::iterator   it;
    std::string::iterator   it2;

    key = line.substr(0, pos);
    line.erase(0, pos + 1);
    while ((pos = line.find(',')) != std::string::npos) {
        tmp =  trim_sides(line.substr(0, pos));
        lstTmp.push_back(tmp);
        line.erase(0, pos + 1);
    }
    tmp = trim_sides(line.substr(0, pos));
    lstTmp.push_back(tmp);
    line.erase(0, pos + 1);
    this->_fields.insert({key, lstTmp});
}

//header parsing
void Request::_parse_header(std::istringstream &raw) {
    std::string             line;
    size_t                  pos = 0;

    getline(raw, line);
    while (!line.empty() && line != "\r") {
        pos = line.find(':');
        if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
        _split_field(pos, line);
        getline(raw, line);
    }
}

//body parsing
void Request::_parse_body(std::istringstream &raw) {
    std::string             line;

    while (getline(raw, line)) {
        this->_body += line;
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
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
