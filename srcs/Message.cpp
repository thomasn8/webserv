#include "../includes/Message.hpp"

// ---------Constructor and destructor ------------

Message::Message() : _body(){
    
}

Message::Message(const Message& instance) {
    *this = instance;
}

Message::~Message() {

}

// --------- Fonctions ------------

std::map<std::string, std::list<std::string>> Message::get_fields() const {
    return this->_fields;
}

// display fields in the terminal
void Message::display_fields() const {
    std::map<std::string, std::list<std::string>>::const_iterator it;
    std::list<std::string>::const_iterator it2;

    for (it = this->_fields.begin(); it != this->_fields.end(); it++) {
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

std::string Message::get_body() const {
    return this->_body;
}

bool Message::has_body() const {
    return this->_body.size();
}

// --------- Operator overload ------------

Message &Message::operator=(const Message &instance) {
    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
