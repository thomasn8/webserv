#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string *rawMessage, Server *server) : 
_rawMessage(rawMessage), _server(server) {

	std::cout << *rawMessage << std::endl;

	ssize_t i = this->_rawMessage->find_first_of('\n');
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
	size_t len = 0;
	const char * lastChar = &(*(this->_rawMessage->end()-1));
    for (it = this->_rawMessage->begin(); it != this->_rawMessage->end() && len < MHS; it++) {
		// std::cout << *it;																	// A TESTER AVEC UNE REQUEST QUI A UN BODY
        if (*it == '\r')
		{
			if ( &(*(it+1)) <= lastChar && *(it + 1) != '\n') 
				*it = ' ';
			else
			{
				// si on trouve le pattern /r/n/r/n c'est qu'on est plus dans le header
				if ( &(*(it+3)) <= lastChar && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
					return;
			}
		}
		len++;
    }
	if (len == MHS)
		throw MessageException(HEADERS_TOO_LARGE);
}

void Request::_parse_start_line(std::string startLine) {
	std::string token;
    ssize_t pos = 0;

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
	ssize_t len = 0;
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
	ssize_t pos = 0, i;
	i = this->_rawMessage->find_first_of('\n');
	while (i != std::string::npos && (*this->_rawMessage).c_str()[0] != '\r')
	{
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		if (this->_rawMessage->c_str()[i-1] != '\r')
            throw MessageException(BAD_REQUEST);
		pos = this->_rawMessage->find(':');
		if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
		_split_field(pos, i-1);	// prend pas le /r
		this->_rawMessage->erase(0, i+1);
		i = this->_rawMessage->find_first_of('\n');
	}
	if ((*this->_rawMessage).c_str()[0] == '\r')
		this->_rawMessage->erase(0, i+1); // efface la derniere ligne vide du header
	else
		throw MessageException(BAD_REQUEST);
	return this->_rawMessage->size(); // retourne la size du body
}

// regarde dans le location correspondant a l'extension de la targert si le type de fichier uploade est valide etaccepte
bool Request::_check_filetype(std::string &contentType)
{
	std::string ext = _target.substr(_target.find_last_of('.')+1, std::string::npos);
	for (std::deque<Location>::iterator it = _server->get_locations().begin(); it != _server->get_locations().end(); it++)
	{
		if (ext == (*it).get_cgi())
		{
			for (std::list<std::string>::iterator it2 = (*it).get_contentTypes().begin(); it2 != (*it).get_contentTypes().end(); it2++)
			{
				if (contentType == (*it2))
					return true;
			}
			break;
		}
	}
	return false;
}


void Request::_parse_body() {
	this->_body = this->_rawMessage;
	
	// faire les checks necessaire sur la len
	mapit contentlen = _fields.find("Content-Length");
	if ((*contentlen).second.size() != 1)						// SI CONTENT-LENGTH PAS PRECISE, BAD REQUEST ?
		throw MessageException(BAD_REQUEST);
	size_t contentLength = strtoul((*contentlen).second.front().c_str(), NULL, 0);
	if (contentLength != _rawMessage->size())
		throw MessageException(BAD_REQUEST);

	// choper le type de donner et agir en consequence
	mapit type = _fields.find("Content-Type");
	if ((*type).second.size() > 1)
		throw MessageException(BAD_REQUEST);
	if ((*type).second.front().c_str()[0] == 'm') // multipart
	{
		ssize_t pos = (*type).second.front().find_first_of('=');
		if (pos == -1)
			throw MessageException(BAD_REQUEST);
		std::string boundry = (*type).second.front().substr(pos+1, std::string::npos);
		
		// std::cout << "BOUNDRY=" << boundry << std::endl;
		// utiliser boundry pour decouper le body
// D'ABORD REUSSIR A FAIRE TOURNER LA LOOP EN PRINTANT CORRECTEMENT CHAQUE BLOQUE
		ssize_t next = 0, boundrylen = boundry.size(), len = 0, namelen = 0, filenamelen = 0, valuelen = 0, contenttypename = 0;
		bool stop = false;
		while (next > -1)
		{
			// CHECK NEW BOUDRY FLAG
			if (this->_rawMessage->find(boundry) != 0)										// check si boundry en debut de bloque
				throw MessageException(BAD_REQUEST);
			else
				this->_rawMessage->erase(0, boundrylen);									// et l'efface

			// FIND NEXT FLAG OR ENDIND FLAG boudry--
			next = this->_rawMessage->find(boundry);										// find next boudry pos
			const char *bodyLastChar = &(*(this->_rawMessage->end()-1));
			const char *boudrynext = this->_rawMessage->c_str() + next + boundrylen + 1;
			if (next = -1 || boudrynext > bodyLastChar)										// si aucun, error
				throw MessageException(BAD_REQUEST);
			else if (*boudrynext == '-')													// si boudry-- parsing du body se termine apres ce block de data
				stop = true;
			
			// TREATE DATA UNTIL NEXT BOUNDRY: next
			std::cout << std::string(this->_rawMessage->c_str(), next);
			if (stop == true)
				break;
			// // si input classic: name-value, si file: name-filename
			// namelen = ;
			// valuelen = ;
			// // si file: type-value
			// filenamelen = ;
			// contenttypename = ;
			// // if (filenamelen && _check_filetype(std::string(...)) == false)	// si upload, choper le filetype
			// // 	throw MessageException(MEDIA_UNSUPPORTED);
			
			// ERASE USED DATA UNTIL NEXT BOUNDRY
			this->_rawMessage->erase(0, next);
		}
		this->_rawMessage->clear();
	}
	else // default/application
	{
		ssize_t i, keylen = 0, vallen = 0;
		i = this->_rawMessage->find_first_of('&');
		while (i != -1)
		{
			// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
			keylen = this->_rawMessage->find_first_of('=');
			vallen = this->_rawMessage->find_first_of('&') - keylen - 1;
			_postNameValue.insert(std::make_pair(std::string(this->_rawMessage->c_str(), keylen), std::string(this->_rawMessage->c_str()+keylen+1, vallen)));
			this->_rawMessage->erase(0, i+1);
			i = this->_rawMessage->find_first_of('&');
		}
		keylen = this->_rawMessage->find_first_of('=');
		vallen = this->_rawMessage->size() - keylen - 1;
		_postNameValue.insert(std::make_pair(std::string(this->_rawMessage->c_str(), keylen), std::string(this->_rawMessage->c_str()+keylen+1, vallen)));
		this->_rawMessage->clear();
	}
}

// --------- Operator overload ------------

Request &Request::operator=(const Request &instance) {
    this->_server = instance._server;
    this->_rawMessage = instance._rawMessage;
    this->_method = instance._method;
    this->_target = instance._target;
    this->_version = instance._version;
	this->_postNameValue = instance._postNameValue;
	this->_postNameFilename = instance._postNameFilename;
	this->_postTypeValue = instance._postTypeValue;

    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
