#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <cstring>
#include <ctime>

std::string     trim_sides(std::string str);
bool            is_number(const std::string& s);

static const char *DAY_NAMES[] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

char            *Rfc1123_DateTimeNow();

#endif