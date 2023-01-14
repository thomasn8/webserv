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
	std::cout << "TEST PARSE CR OKAY" << std::endl;
    _parse_start_line(start_line);
	std::cout << "TEST PARSE START LINE OKAY" << std::endl;
    if (_parse_header() > 0)
	{
		std::cout << "HAS BODY" << std::endl;
    	_parse_body();
	}
	std::cout << "TEST REQUEST OBJECT OKAY" << std::endl;
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
	const void * lastChar = static_cast<const void *>(&(*(this->_rawMessage->rbegin()))); // chope l'adresse du dernier char de _rawMessage (protection)
    for (it = this->_rawMessage->begin(); it != this->_rawMessage->end() && len < MHS; it++) {
        if (*it == '\r')
		{
			if (static_cast<const void *>(&(*(it+3))) > lastChar) // protection pour voir si memoire est accessible vu qu'on teste *(it + 3) en bas
				throw MessageException(BAD_REQUEST);
			if (*(it + 1) != '\n') 
				*it = ' ';
			else
			{
				// si on trouve le pattern /r/n/r/n c'est qu'on est plus dans le header
				if (*(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
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
	std::cout << "TEST PARSE HEADER OKAY" << std::endl;
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

std::string find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop)
{
	ssize_t namestart = block.find(strtofind) + strlen(strtolen);
	ssize_t nameend = block.find(stop, namestart);
	ssize_t namelen = nameend - namestart;
	return std::string(block.c_str() + namestart, namelen);
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
		std::string boundry = ((*type).second.front().substr(pos+1, std::string::npos)).insert(0, "--");
		ssize_t next = 0, boundrylen = boundry.size(), first = this->_rawMessage->find(boundry);
		if (first == -1)
			throw MessageException(BAD_REQUEST);
		this->_rawMessage->erase(0, first); // efface les caracteres jusqu au 1er boundry (\r \n ou whitespace)
		while (1)
		{
			// check si boundry en debut de block et l'efface
			if (this->_rawMessage->find(boundry) != 0)
				throw MessageException(BAD_REQUEST);
			else
				this->_rawMessage->erase(0, boundrylen);

			// find next boudry or break
			next = this->_rawMessage->find(boundry);
			if (next == -1)
				break;
			
			// TREATE DATA UNTIL NEXT BOUNDRY
			// std::cout << boundry << std::string(this->_rawMessage->c_str(), next);	// check le contenu si correspond a la requete
			// std::cout << "\n\nblock: |" << std::string(this->_rawMessage->c_str(), next) << "|" << std::endl;
			ssize_t start_secondline = this->_rawMessage->find('\n', 2) + 1;
			std::string first_line = this->_rawMessage->substr(2, start_secondline - 4); // attention au CR
			// name
			std::cout << "name=|" << find_value_from_boundry_block(first_line, "name=", "name=\"", '"') << "|" << std::endl;
			// si file: filename + type
			// std::cout << "first line = |" << first_line << "|" << std::endl;
			if (first_line.find("filename=") != -1)
			{
				// filename
				std::cout << "filename=|" << find_value_from_boundry_block(first_line, "filename=", "filename=\"", '"') << "|" << std::endl;

				// contenttype
				ssize_t end_secondline = this->_rawMessage->find('\r', start_secondline);
				// std::cout << "char:" << this->_rawMessage->c_str()[end_secondline] << ", i=" << end_secondline << std::endl;
				std::string second_line = this->_rawMessage->substr(start_secondline, end_secondline); // attention au CR
				// std::cout << "second line from " << start_secondline << " to " <<  end_secondline << ": |" << second_line << "|" << std::endl;
				std::cout << "contenttype=|" << find_value_from_boundry_block(second_line, "Content-Type:", "Content-Type: ", '\r') << "|"<< std::endl;

				// check file extension if server accept
				// if (_check_filetype(std::string(...)) == false)	// si upload, choper le filetype
				// 	throw MessageException(MEDIA_UNSUPPORTED);
			}
			// value
			// ssize_t valuepos = this->_rawMessage->find("name=") + strlen("name=\"");
			// ssize_t valuelen = ;
			
			// sinon efface le contenu jusquau next boundry
			this->_rawMessage->erase(0, next);
		}
		// CLEAR LAST FLAG
		// std::cout << "end of body: " << *this->_rawMessage << std::endl; // check si on a les -- residuels du dernier boundry
		this->_rawMessage->clear();
	}
	else // default/application
	{
		ssize_t i, keylen = 0, vallen = 0;
		i = this->_rawMessage->find('&');
		while (i != -1)
		{
			// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
			keylen = this->_rawMessage->find('=');
			vallen = this->_rawMessage->find('&') - keylen - 1;
			_postNameValue.insert(std::make_pair(std::string(this->_rawMessage->c_str(), keylen), std::string(this->_rawMessage->c_str()+keylen+1, vallen)));
			this->_rawMessage->erase(0, i+1);
			i = this->_rawMessage->find('&');
		}
		keylen = this->_rawMessage->find('=');
		vallen = this->_rawMessage->size() - keylen - 1;
		_postNameValue.insert(std::make_pair(std::string(this->_rawMessage->c_str(), keylen), std::string(this->_rawMessage->c_str()+keylen+1, vallen)));
		this->_rawMessage->clear();
	}
	std::cout << "TEST PARSE BODY OKAY" << std::endl;
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
