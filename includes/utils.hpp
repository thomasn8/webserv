#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <cstring>
#include <ctime>
#include <iostream>

std::string     trim_sides(std::string str);
bool            is_number(const std::string& s);
void			highlight_crlf(const char *block, ssize_t size);

static const char *DAY_NAMES[] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

void Rfc1123_DateTimeNow(std::string &date);

#endif