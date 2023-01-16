#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string *rawMessage, Server *server) : _rawMessage(rawMessage), _server(server) {
	ssize_t i = _rawMessage->find_first_of('\n');
    std::string start_line = _rawMessage->substr(0, i); // prend le /r avant /n
	_rawMessage->erase(0, i+1);
    _replace_alone_header_cr();
    _parse_start_line(start_line);
    if (_parse_header() > 0)
    	_parse_body();
}

Request::Request(const Request& instance) :
_rawMessage(instance._rawMessage),
_server(instance._server),
_method(instance._method),
_target(instance._target),
_version(instance._version),
_body(instance._body),
_body_len(instance._body_len),
_fields(instance._fields),
_postNameValue(instance._postNameValue),
_postMultipart(instance._postMultipart) 
{}

Request::~Request() {
	_free_multipartDatas();
}

// --------- Getters/Setters ------------

std::string Request::get_method() const { return _method; }

std::string Request::get_target()const { return _target; }

std::string Request::get_version() const { return _version; }

std::map<std::string, std::list<std::string>> Request::get_fields() const { return _fields; }

std::map<std::string, std::string> & Request::get_defaultDatas() { return _postNameValue; }

std::list<MultipartData *> & Request::get_multipartDatas() { return _postMultipart; }

// --------- Parse HEADER ------------

void Request::_replace_alone_header_cr() {
    std::string::iterator it;
	size_t len = 0;
	const void * lastChar = static_cast<const void *>(&(*(_rawMessage->rbegin()))); // chope l'adresse du dernier char de _rawMessage (protection)
    for (it = _rawMessage->begin(); it != _rawMessage->end() && len < MHS; it++) {
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

	if (startLine.back() != '\r')
		 throw MessageException(BAD_REQUEST);
	else
		startLine.pop_back();
	for(int i = 0; i < 3; i++) {
        pos = startLine.find(' ');
        if (i < 2 && pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
        if (i == 0) {
            _method = startLine.substr(0, pos);
            if (!(_method == "GET" || _method == "POST" || _method == "DELETE"))
                throw MessageException(METHOD_NOT_ALLOWED);
        }
        else if (i == 1)
            _target = startLine.substr(0, pos);
        else if (i == 2) {
            _version = startLine.substr(0, pos);
            if (_version.compare("HTTP/1.1") != 0)
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

// regarde dans le location correspondant a l'extension de la target si le type de fichier uploade est accepte
bool Request::_check_filetype(std::string contentType) {
	size_t slash = contentType.rfind('/');
	if (slash != -1)
		contentType.erase(0, slash + 1);
	std::string ext = _target.substr(_target.find_last_of('.')+1, std::string::npos);
	for (Server::loc_it it = _server->get_locations().begin(); it != _server->get_locations().end(); it++)
	{
		if (ext == (*it).get_route())
		{
			std::list<std::string>::iterator it2 = (*it).get_contentTypes().begin();
			for (; it2 != (*it).get_contentTypes().end(); it2++)
			{
				if (contentType == (*it2))
					return true;
			}
			break;
		}
	}
	return false;
}

std::string Request::_find_value_from_boundry_block(std::string &block, const char *strtofind, const char *strtolen, char stop) {
	ssize_t valstart = block.find(strtofind) + strlen(strtolen);
	ssize_t valend = block.find(stop, valstart);
	ssize_t vallen = valend - valstart;
	return std::string(block.c_str() + valstart, vallen);
}

void Request::_parse_defaultDataType() {
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

void Request::_parse_multipartDataType(fields_it type) {
	ssize_t pos = (*type).second.front().find_first_of('=');
	if (pos == -1)
		throw MessageException(BAD_REQUEST);
	std::string boundry = ((*type).second.front().substr(pos+1, std::string::npos)).insert(0, "--");
	ssize_t next = 0, boundrylen = boundry.size(), first = _rawMessage->find(boundry);
	size_t start_value = 0;
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
		std::string name = _find_value_from_boundry_block(first_line, "name=", "name=\"", '"');
		MultipartData *multi = new MultipartData(name);
		if (first_line.find("filename=") != -1)	// FILE SPECIFIC
		{
			multi->set_fileName(_find_value_from_boundry_block(first_line, "filename=", "filename=\"", '"'));
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
		}
		else
		{
			ssize_t start_thirdline = start_secondline + 2;
			start_value = start_thirdline;
		}
		size_t valuelen = next - start_value - 2;
		multi->set_valueLen(valuelen);
		if (valuelen > 0)
			multi->set_value(std::string(&_rawMessage->c_str()[start_value], valuelen));
		
		// INSERT ET EFFACE LE CONTENU DU BODY JUSQUAU NEXT BOUNDRY
		_postMultipart.push_back(multi);
		_rawMessage->erase(0, next);
	}
	// CLEAR LAST FLAG
	_rawMessage->clear();
	_print_multipartDatas();
}

void Request::_parse_body() {
	// faire les checks necessaire sur la len
	fields_it contentlen = _fields.find("Content-Length");
	_display_fields();
	if ((*contentlen).second.size() != 1)
		throw MessageException(BAD_REQUEST);
	size_t contentLength = strtoul((*contentlen).second.front().c_str(), NULL, 0);
	if (contentLength != _rawMessage->size())
		throw MessageException(BAD_REQUEST);
	_body = _rawMessage->c_str();
	_body_len = contentLength;

	// choper le type de donner et parser en fonction
	fields_it type = _fields.find("Content-Type");
	if ((*type).second.size() > 1)
		throw MessageException(BAD_REQUEST);
	if ((*type).second.front().c_str()[0] == 'm')
		_parse_multipartDataType(type);
	else
		_parse_defaultDataType();
}

// delete les Multipart * alloues dans map de _postMultipart
void Request::_free_multipartDatas() {
	if (_postMultipart.size() > 0)
	{
		std::list<MultipartData *>::iterator it = _postMultipart.begin();
		for (; it != _postMultipart.end(); it++)
			delete (*it);
	}
}

// --------- Print datas ------------

void Request::_display_fields() const {
    fields_it it;
    fields_values_it it2;
    for (it = _fields.begin(); it != _fields.end(); it++) {
        std::cout << it->first;
        std::cout << ": ";
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            std::cout << *it2;
            if (it2 != std::prev(it->second.end()))
                std::cout << ", ";
        }
        std::cout << ";" << std::endl;
    }
}

void Request::_print_defaultDatas() const {
	std::cout << "\nPOST APPLICATION DATAS" << std::endl;
	if (_postNameValue.size() > 0)
	{
		post_mapit it = _postNameValue.cbegin();
		for (; it != _postNameValue.cend(); it++)
		{
			std::cout << "Data:" << std::endl;
			std::cout << "	name = |" << (*it).first << "|" << std::endl;
			std::cout << "	value = |" << (*it).second << "|" << std::endl;
		}
		std::cout << std::endl;
	}
}

void Request::_print_multipartDatas() const {
	std::cout << "\nPOST MULTIPART DATAS" << std::endl;
	if (_postMultipart.size() > 0)
	{
		post_listit it = _postMultipart.cbegin();
		for (; it != _postMultipart.cend(); it++)
		{
			std::cout << "Data (" << static_cast<const void *>(*it) << "):" << std::endl;
			std::cout << "	name = |" << (*it)->get_name() << "|" << std::endl;
			if ((*it)->get_file() == true)
			{
				std::cout << "	filename = |" << (*it)->get_fileName() << "|" << std::endl;
				std::cout << "	content type = |" << (*it)->get_contentType() << "|" << std::endl;
			}
			if ((*it)->get_value() != NULL)
			{
				size_t len = (*it)->get_valueLen();
				const char * ptr = (*it)->get_value();
				std::cout << "	value = |";
				for (int i = 0; i < len; i++)
					std::cout << ptr[i];
				std::cout << "|" << std::endl;
			}
		}
		std::cout << std::endl;
	}
}
