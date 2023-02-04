#ifndef RESPONSEINFOS_HPP
# define RESPONSEINFOS_HPP

#include <string>

struct responseInfos
{
	std::string	header;
	char		*body;
	size_t		body_size;
};

// struct responseInfos
// {
// 	char 		*header;
// 	size_t 		header_size;
// 	char		*body;
// 	size_t		body_size;
// };

#endif
