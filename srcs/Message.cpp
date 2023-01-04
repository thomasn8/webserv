#include "../includes/Message.hpp"

// ---------Constructor and destructor ------------

Message::Message() {
    
}

Message::Message(const Message& instance) {
    *this = instance;
}

Message::~Message() {

}

// --------- Fonctions ------------

std::map<std::string, std::string> Message::get_fields() const {
    return this->_fields;
}

std::string Message::get_body() const {
    return this->_body;
}

bool Message::has_body() const {
    return this->_hasBody;
}

// --------- Operator overload ------------

Message &Message::operator=(const Message &instance) {
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
