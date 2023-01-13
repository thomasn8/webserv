#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string *rawMessage) : _rawMessage(rawMessage) {	// passe un ptr au lieu d un string pour eviter 1 copie
// std::cout << "ALL:\n" << *rawMessage;
	// chope la 1ere ligne sans recopier une 2eme fois toute la request dans un istringstream
	size_t i = this->_rawMessage->find_first_of('\n');
// std::cout << "_rawMessage[i-1]: |" << this->_rawMessage->c_str()[i-1] << "|" << std::endl;
// std::cout << "_rawMessage[i]: |" << this->_rawMessage->c_str()[i] << "|" << std::endl;
// std::cout << "_rawMessage[i+1]: |" << this->_rawMessage->c_str()[i+1] << "|" << std::endl;
    // std::string start_line = this->_rawMessage->substr(0, i-1); // prend pas le /r avant /n
    std::string start_line = this->_rawMessage->substr(0, i); // prend le /r avant /n
std::cout << "STARTLINE:\n" << start_line;
	_rawMessage->erase(0, i+1);
std::cout << "\nRESTE:\n" << *rawMessage;
    
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
        if (*it == '\r')
		{
			if (it+1 < (*this->_rawMessage).end() && *(it + 1) != '\n') 
				*it = ' ';
			else
			{
				if (it+3 < (*this->_rawMessage).end() 
					&& *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n') // si on trouve le pattern /r/n/r/n c'est qu'on est plus dans le header
					return;
			}
		}
    }
}

//start-line parsing
void Request::_parse_start_line(std::string startLine) {
	std::string token;
    size_t pos = 0;

	if (*(startLine.end()-1) == '\r')
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

void Request::_split_field(size_t separator, size_t lastchar) {				// A TESTER
// std::cout << "SPLIT_FIELD:" << this->_rawMessage->substr(0,lastchar) << std::endl;
// std::cout << "KEY:" << std::string(_rawMessage->c_str(), separator) << std::endl;
	std::list<std::string> listValues;	
	std::string key(_rawMessage->c_str(), separator);
	const char *values = _rawMessage->c_str()+separator+1;
// std::cout << "VALUES:" << std::string(values, lastchar) << std::endl;
	const char *newvalue = values;
	int len = 0;
	while (*values)
	{
		if (*values == ',')
		{
// std::cout << "NEW VALUE:" << trim_sides(std::string(newvalue, len)) << std::endl;
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
// std::cout << "NEW VALUE:" << trim_sides(std::string(newvalue, len)) << std::endl;
	listValues.push_back(trim_sides(std::string(newvalue, len)));
	this->_fields.insert(std::make_pair(key, listValues));
}

int Request::_parse_header() {
	size_t pos = 0, i = std::string::npos;
	i = this->_rawMessage->find_first_of('\n');
	while (i != std::string::npos && *this->_rawMessage != "\r\n")
	{

if (i != std::string::npos && *this->_rawMessage != "\r\n")
	std::cout << "Line=" << this->_rawMessage->substr(0,i) << std::endl; // affiche la ligne avec le /r, sans le /n
		
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		pos = this->_rawMessage->find(':');
		if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
		_split_field(pos, i-1); // passe le RESTE avec l'index du separator et l'index du dernier char avant le \r
		this->_rawMessage->erase(0, i+1);
		i = this->_rawMessage->find_first_of('\n');

std::cout << "RESTE:\n" << *_rawMessage;
	}

std::cout << "LAST LINE FOUND\n" << *_rawMessage;
	if (*this->_rawMessage == "\r\n")
	{
std::cout << "i = " << i << ": erase last empty line of header\n";
		this->_rawMessage->erase(0, i+1);	// efface la derniere ligne vide du header
	}

std::cout << "\nFIELDS:\n";
display_fields();
	return this->_rawMessage->size();
}

//body parsing													// pas top car c'est pas vraiment un parsing mais juste une copie de la memoire
// void Request::_parse_body(std::istringstream &raw) {			// qui peut etre lourde selon la longueur du body
//     std::string             line;

//     while (getline(raw, line)) {
//         this->_body += line;
//     }
// }
void Request::_parse_body() {	// si aucune erreur dans la maniere dont je parse le header, on a plus qu'a utiliser le ptr sur le reste de _rawMessage variable _body
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
