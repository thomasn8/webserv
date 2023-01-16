#include "../includes/MultipartData.hpp"

/* 
	************ CONST/DESTR
*/
MultipartData::MultipartData(std::string & name) : 
_name(name), _value(NULL), _valueLen(0), _file(false) 
{}

MultipartData::MultipartData(MultipartData const &instance) :
_name(instance._name), _value(instance._value), _valueLen(instance._valueLen),
_file(instance._file), _fileName(instance._fileName), _contentType(instance._contentType) 
{}

MultipartData::~MultipartData() {}

/* 
	************ GETTERS/SETTERS
*/
void MultipartData::set_name(std::string name) { _name = name; }

void MultipartData::set_value(const char *value) { _value = value; }

void MultipartData::set_valueLen(size_t value_len) { _valueLen = value_len; }

void MultipartData::set_file(bool file) { _file = file; }

void MultipartData::set_fileName(std::string filename) { _fileName = filename; }

void MultipartData::set_contentType(std::string contenttype) { _contentType = contenttype;}

std::string MultipartData::get_name() const { return _name; }

const char * MultipartData::get_value() const { return _value; }

size_t MultipartData::get_valueLen() const { return _valueLen; }

bool MultipartData::get_file() const { return _file; }

std::string MultipartData::get_fileName() const { return _fileName; }

std::string MultipartData::get_contentType() const { return _contentType; }

/* 
	************ PRINT
*/
void MultipartData::print_data() const
{
	std::cout << "Multipart data (" << static_cast<const void *>(this) << "):" << std::endl;
	std::cout << "name = " << get_name() << std::endl;
	if (get_file())
	{
		std::cout << "filename = " << get_fileName() << std::endl;
		std::cout << "content type = " << get_contentType() << std::endl;
	}
	std::cout << "value len = " << get_valueLen() << std::endl;
	const char * ptr = get_value();
	std::cout << "value = ";
	for (int i = 0; i < get_valueLen(); i++)
		std::cout << ptr[i];
	std::cout << std::endl;
}
