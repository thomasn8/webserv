#include "../includes/Response.hpp"

// ---------Constructor and destructor ------------

Response::Response(Request &request) : _request(request) {
    
}

Response::Response(const Response& instance) : _request(instance._request) {
    *this = instance;
}

Response::~Response() {

}

// --------- Fonctions ------------

// std::string Response::getMessage() const {
//     return this->_rowMessage;
// }

// std::string Response::getMethod() const {
//     return this->_method;
// }

// std::string Response::getTarget() const {
//     return this->_target;
// }

// std::string Response::getVersion() const {
//     return this->_version;
// }

// --------- Operator overload ------------

Response &Response::operator=(const Response &instance) {
    // this->_rowMessage = instance._rowMessage;
    // this->_method = instance._method;
    // this->_target = instance._target;
    // this->_version = instance._version;

    // this->_hasBody = instance._hasBody;
    // this->_body = instance._body;

    // std::map<std::string, std::string>::const_iterator it;
    // std::map<std::string, std::string>::const_iterator it2;
    // it2 = this->_fields.begin();
    // for (it = instance._fields.begin(); it != instance._fields.end(); it++) {
    //     it2 = it;
    //     it2++;
    // }
    return *this;
}
