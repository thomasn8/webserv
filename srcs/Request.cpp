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
	_free_multipartDatas();
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
	_fields.insert(std::make_pair(key, listValues));
}

int Request::_parse_header() {
	ssize_t pos = 0, i;
	i = _rawMessage->find_first_of('\n');
	while (i != std::string::npos && (*_rawMessage).c_str()[0] != '\r')
	{
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		if (_rawMessage->c_str()[i-1] != '\r')
            throw MessageException(BAD_REQUEST);
		pos = _rawMessage->find(':');
		if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
		_split_field(pos, i-1);	// prend pas le /r
		_rawMessage->erase(0, i+1);
		i = _rawMessage->find_first_of('\n');
	}
	if ((*_rawMessage).c_str()[0] == '\r')
		_rawMessage->erase(0, i+1); // efface la derniere ligne vide du header
	else
		throw MessageException(BAD_REQUEST);
	return _rawMessage->size(); // retourne la size du body
}

// regarde dans le location correspondant a l'extension de la targert si le type de fichier uploade est accepte
bool Request::_check_filetype(std::string contentType)
{
	size_t slash = contentType.rfind('/');
	if (slash != -1)
		contentType.erase(0, slash + 1);
	std::string ext = _target.substr(_target.find_last_of('.')+1, std::string::npos);
	for (Server::loc_it it = _server->get_locations().begin(); it != _server->get_locations().end(); it++)
	{
		if (ext == (*it).get_route())
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

std::string Request::_find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop)
{
	ssize_t valstart = block.find(strtofind) + strlen(strtolen);
	ssize_t valend = block.find(stop, valstart);
	ssize_t vallen = valend - valstart;
	return std::string(block.c_str() + valstart, vallen);
}

void Request::_parse_defaultDataType()
{
	ssize_t i, keylen = 0, vallen = 0;
	i = _rawMessage->find('&');
	while (i != -1)
	{
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		keylen = _rawMessage->find('=');
		vallen = _rawMessage->find('&') - keylen - 1;
		_postNameValue.insert(std::make_pair(std::string(_rawMessage->c_str(), keylen), std::string(_rawMessage->c_str()+keylen+1, vallen)));
		_rawMessage->erase(0, i+1);
		i = _rawMessage->find('&');
	}
	keylen = _rawMessage->find('=');
	vallen = _rawMessage->size() - keylen - 1;
	_postNameValue.insert(std::make_pair(std::string(_rawMessage->c_str(), keylen), std::string(_rawMessage->c_str()+keylen+1, vallen)));
	_rawMessage->clear();
}

void Request::_parse_multipartDataType(mapit type)
{
	ssize_t pos = (*type).second.front().find_first_of('=');
	if (pos == -1)
		throw MessageException(BAD_REQUEST);
	std::string boundry = ((*type).second.front().substr(pos+1, std::string::npos)).insert(0, "--");
	ssize_t next = 0, boundrylen = boundry.size(), first = _rawMessage->find(boundry);
	size_t start_value = 0;
	const char *valueptr;
	if (first == -1)
		throw MessageException(BAD_REQUEST);
	_rawMessage->erase(0, first); // efface les \r \n jusqu au 1er boundry
	while (1)
	{
		// CHECK SI BOUNDRY EN DEBUT DE BLOCK ET L'EFFACE
		if (_rawMessage->find(boundry) != 0)
			throw MessageException(BAD_REQUEST);
		_rawMessage->erase(0, boundrylen);
		// FIND NEXT BOUDRY OR BREAK
		next = _rawMessage->find(boundry);
		if (next == -1)
			break;
		// TREATE DATA UNTIL NEXT BOUNDRY (attention au CRCL dans les boundry block)
		ssize_t start_secondline = _rawMessage->find('\n', 2) + 1;
		std::string first_line = _rawMessage->substr(2, start_secondline - 4);
		std::cout << std::endl << "name=|" << _find_value_from_boundry_block(first_line, "name=", "name=\"", '"') << "|" << std::endl;
		std::string name = _find_value_from_boundry_block(first_line, "name=", "name=\"", '"');
		MultipartData *multi = new MultipartData();
		if (first_line.find("filename=") != -1)
		{
			multi->set_fileName(_find_value_from_boundry_block(first_line, "filename=", "filename=\"", '"'));
			// std::cout << "filename=|" << _find_value_from_boundry_block(first_line, "filename=", "filename=\"", '"') << "|" << std::endl;
			if (multi->get_fileName().size())
				multi->set_file(true);
			ssize_t end_secondline = _rawMessage->find('\r', start_secondline);
			std::string second_line = _rawMessage->substr(start_secondline, end_secondline);
			if (multi->get_file())
			{
				multi->set_contentType(_find_value_from_boundry_block(second_line, "Content-Type:", "Content-Type: ", '\r'));
				if (_check_filetype(multi->get_contentType()) == false)
					throw MessageException(MEDIA_UNSUPPORTED);
			}
			ssize_t start_fourthline = end_secondline + 4;
			start_value = start_fourthline;
			valueptr = &_rawMessage->c_str()[start_value];
		}
		else
		{
			ssize_t start_thirdline = start_secondline + 2;
			start_value = start_thirdline;
			valueptr = &_rawMessage->c_str()[start_value];
		}
		size_t valuelen = next - start_value - 2;
		// std::cout << "value=|" << std::string(valueptr, valuelen) << "|" << std::endl;
		multi->set_valueLen(valuelen);
		if (valuelen > 0)
			multi->set_value(valueptr);
		// INSERT ET EFFACE LE CONTENU DU BODY JUSQUAU NEXT BOUNDRY
		multi->print_data();
		_postMultipart.insert(std::make_pair(name, multi));
		_rawMessage->erase(0, next);
	}
	// CLEAR LAST FLAG
	_rawMessage->clear();
}

void Request::_parse_body() {
	_body = _rawMessage;
	
	// faire les checks necessaire sur la len
	mapit contentlen = _fields.find("Content-Length");
	if ((*contentlen).second.size() != 1)
		throw MessageException(BAD_REQUEST);
	size_t contentLength = strtoul((*contentlen).second.front().c_str(), NULL, 0);
	if (contentLength != _rawMessage->size())
		throw MessageException(BAD_REQUEST);

	// choper le type de donner et parser en fonction
	mapit type = _fields.find("Content-Type");
	if ((*type).second.size() > 1)
		throw MessageException(BAD_REQUEST);
	if ((*type).second.front().c_str()[0] == 'm')
		_parse_multipartDataType(type);
	else
		_parse_defaultDataType();

	_print_defaultDatas();
	_print_multipartDatas();
}

void Request::_print_defaultDatas() const
{
	std::cout << "\nPRINT BODY POST APPLICATION DATAS" << std::endl;
	if (_postNameValue.size() > 0)
	{
		std::map<std::string, std::string>::const_iterator it = _postNameValue.cbegin();
		for (; it != _postNameValue.cend(); it++)
		{
			std::cout << "Data:" << std::endl;
			std::cout << "	name = |" << (*it).first << "|" << std::endl;
			std::cout << "	value = |" << (*it).second << "|" << std::endl;
		}
		std::cout << std::endl;
	}
}

void Request::_print_multipartDatas() const
{
	std::cout << "\nPRINT BODY POST MULTIPART DATAS" << std::endl;
	if (_postMultipart.size() > 0)
	{
		std::map<std::string, MultipartData *>::const_iterator it = _postMultipart.cbegin();
		for (; it != _postMultipart.cend(); it++)
		{
			std::cout << "Data:" << std::endl;
			std::cout << "	name = |" << (*it).first << "|" << std::endl;
			if ((*it).second->get_file() == true)
			{
				std::cout << "	filename = |" << (*it).second->get_fileName() << "|" << std::endl;
				std::cout << "	content type = |" << (*it).second->get_contentType() << "|" << std::endl;
			}
			if ((*it).second->get_value() != NULL)
			{
				size_t len = (*it).second->get_valueLen();
				const char * ptr = (*it).second->get_value();
				std::cout << "	value = |";
				for (int i = 0; i < len; i++)
					std::cout << ptr[i];
				std::cout << "|" << std::endl;
			}
		}
		std::cout << std::endl;
	}
}

// delete les Multipart * alloues dans map de _postMultipart
void Request::_free_multipartDatas()
{
	if (_postMultipart.size() > 0)
	{
		std::map<std::string, MultipartData *>::const_iterator it = _postMultipart.cbegin();
		for (; it != _postMultipart.cend(); it++)
			delete (*it).second;
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
	this->_postMultipart = instance._postMultipart;

    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
