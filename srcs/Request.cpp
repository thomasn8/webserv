#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string *rawMessage) : _rawMessage(rawMessage) {
	size_t i = this->_rawMessage->find_first_of('\n');
    // std::string start_line = this->_rawMessage->substr(0, i-1); // prend pas le /r avant /n
    std::string start_line = this->_rawMessage->substr(0, i); // prend le /r avant /n
	_rawMessage->erase(0, i+1);    
    _check_alone_CR();
    _parse_start_line(start_line);
    if (_parse_header() > 0)
    	_parse_body();
}

Request::Request(const Request& instance) {
    *this = instance;
}

Request::~Request() {

}

// --------- Fonctions ------------

std::string Request::get_message() const {
    return *this->_rawMessage;
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
void Request::_check_alone_CR() { // parse que le header
    std::string::iterator it;
    for (it = (*this->_rawMessage).begin(); it != (*this->_rawMessage).end(); it++) {
		// std::cout << *it;																	// A TESTER AVEC UNE REQUEST QUI A UN BODY
        if (*it == '\r')
		{
			if (it+1 < (*this->_rawMessage).end() && *(it + 1) != '\n') 
				*it = ' ';
			else
			{
				// si on trouve le pattern /r/n/r/n c'est qu'on est plus dans le header
				if (it+3 < (*this->_rawMessage).end() && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
					return;
			}
		}
    }
}

//start-line parsing
void Request::_parse_start_line(std::string startLine) {
	std::string token;
    size_t pos = 0;

	if (*(startLine.end()-1) != '\r')
		 throw MessageException(BAD_REQUEST);
	else
		startLine.pop_back();

	for(int i = 0; i < 3; i++) {
        pos = startLine.find(' ');
        if (i < 2 && pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
        if (i == 0) {
            this->_method = startLine.substr(0, pos);
            if (!(this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE"))
                throw MessageException(METHOD_NOT_ALLOWED);
        }
        else if (i == 1)
            this->_target = startLine.substr(0, pos);
        else if (i == 2) {
            this->_version = startLine.substr(0, pos);
            if (this->_version.compare("HTTP/1.1") != 0)
                throw MessageException(HTTP_VERSION_UNSUPPORTED);
        }
        startLine.erase(0, pos + 1);
    }
    if (pos != std::string::npos)
        throw MessageException(BAD_REQUEST);
}

void Request::_split_field(size_t separator, size_t lastchar) {
	std::list<std::string> listValues;	
	std::string key(_rawMessage->c_str(), separator);
	const char *values = _rawMessage->c_str()+separator+1;
	const char *newvalue = values;
	size_t newlastchar = lastchar - separator - 1;
	int len = 0;
	while (*values && newlastchar--)
	{
		if (*values == ',')
		{
			listValues.push_back(trim_sides(std::string(newvalue, len)));
			if (*(values+1))
				newvalue = values+1;
			else
				break;
			len = -1;
		}
		values++;
		len++;
	}
	listValues.push_back(trim_sides(std::string(newvalue, len)));
	this->_fields.insert(std::make_pair(key, listValues));
}

int Request::_parse_header() {
	size_t pos = 0, i = std::string::npos;
	i = this->_rawMessage->find_first_of('\n');
	while (i != std::string::npos && *this->_rawMessage != "\r\n")
	{
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		if (this->_rawMessage->c_str()[i-1] != '\r')									// rajouter check pour verifier si chaque fin de ligne du header contient \r
            throw MessageException(BAD_REQUEST);
		pos = this->_rawMessage->find(':');
		if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
		_split_field(pos, i-1);
		this->_rawMessage->erase(0, i+1); // prend pas le /r
		i = this->_rawMessage->find_first_of('\n');
	}
	if (*this->_rawMessage == "\r\n")
		this->_rawMessage->erase(0, i+1); // efface la derniere ligne vide du header
	return this->_rawMessage->size();
}

void Request::_parse_body() {
	this->_body = this->_rawMessage;
	// FAIRE PARSING DU BODY
}

// --------- Operator overload ------------

Request &Request::operator=(const Request &instance) {
    this->_rawMessage = instance._rawMessage;
    this->_method = instance._method;
    this->_target = instance._target;
    this->_version = instance._version;

    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
