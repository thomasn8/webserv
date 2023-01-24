#ifndef MULTIPARTDATA_HPP
# define MULTIPARTDATA_HPP

#include <string>
#include <iostream>

class MultipartData
{
	public:
		MultipartData(std::string &name);
		MultipartData(MultipartData const &instance);
		~MultipartData();
		MultipartData &operator=(MultipartData const &instance);

		void set_name(std::string name);
		void set_value(std::string value);
		void set_valueLen(size_t value_len);
		void set_file(bool file);
		void set_fileName(std::string filename);
		void set_contentType(std::string contenttype);
		
		std::string get_name() const;
		const char * get_value() const;
		size_t get_valueLen() const;
		bool get_file() const;
		std::string get_fileName() const;
		std::string get_contentType() const;
		
		void print_data() const;

	private:
		std::string	_name;
		std::string	_value;
		size_t		_valueLen;
		bool		_file;
		std::string	_fileName;
		std::string	_contentType;
};

#endif
