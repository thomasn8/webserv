#include "../includes/Response.hpp"

// ---------Constructor and destructor ------------

Response::Response(Request &request, Server &server) : _request(request), _server(server),
_finalMessage(std::string()), _version(std::string("HTTP/1.1")), 
_statusCode(std::string()), _reason(std::string()),
_isCGI(true), _targetFound(true) {
    if (request.get_method() == GET)
        _response_get();
    else if (request.get_method() == POST)
        _response_post();
    else if (request.get_method() == DELETE)
        _response_delete();
    else
        throw MessageException(HTTP_VERSION_UNSUPPORTED);  
}

void Response::_response_get() {
    _check_target(this->_request.get_target());

    // 1. est-ce que le path est correcte?
        // 1.1 host-name = server_name
        // 1.2 redirections, locations, root
    // 2. est-ce que c'est un cgi ou non?
    // 3. construire la réponse
}

void Response::_response_post() {

}

void Response::_response_delete() {

}

void Response::_check_target(std::string target) {

    std::cout << target << std::endl;

}

Response::Response(const Response& instance) : _request(instance._request), _server(instance._server) {
    *this = instance;
}

Response::~Response() {

}

// --------- Fonctions ------------

std::string Response::getMessage() const {
    return this->_finalMessage;
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

    this->_hasBody = instance._hasBody;
    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
