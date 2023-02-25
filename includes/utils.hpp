#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <cstring>
#include <ctime>
#include <iostream>
#include <cstdlib>

std::string     trim_sides(std::string str);
bool            is_number(const std::string& s);
void			highlight_crlf(const char *block, ssize_t size);

void Rfc1123_DateTimeNow(std::string &date);

#endif