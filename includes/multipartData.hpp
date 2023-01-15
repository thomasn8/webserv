#ifndef MULTIPARTDATA_HPP
# define MULTIPARTDATA_HPP

#include <string>

class MultipartData
{
	public:
		MultipartData();
		MultipartData(MultipartData const &instance);
		~MultipartData();
		MultipartData &operator=(MultipartData const &instance);

		void set_value(const char *value);
		void set_valueLen(size_t value_len);
		void set_file(bool file);
		void set_fileName(std::string filename);
		void set_contentType(std::string contenttype);
		const char * get_value() const;
		size_t get_valueLen() const;
		bool get_file() const;
		std::string get_fileName() const;
		std::string get_contentType() const;

	private:
		const char *_value;
		size_t _valueLen;
		bool _file;
		std::string _fileName;
		std::string _contentType;
};

#endif
