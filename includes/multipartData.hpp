#ifndef MULTIPARTDATA_HPP
# define MULTIPARTDATA_HPP

#include <string>

struct multipartData
{
	const char *value;
	size_t value_len;
	bool file;
	std::string filename;
	std::string contenttype;
};

#endif
