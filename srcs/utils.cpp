#include "../includes/utils.hpp"

const char *DAY_NAMES[] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char *MONTH_NAMES[] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

std::string trim_sides(std::string str)
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0,str.find_first_not_of(typeOfWhitespaces));
	return str;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Rfc1123_DateTimeNow(std::string &date)
{
    const int RFC1123_TIME_LEN = 29;
    time_t t;
    struct tm tm;
    char * buf = (char *)malloc(RFC1123_TIME_LEN); 

    time(&t);
    gmtime_r(&t, &tm);

    strftime(buf, RFC1123_TIME_LEN, "---, %d --- %Y %H:%M:%S GMT", &tm);
    memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
    memcpy(buf+8, MONTH_NAMES[tm.tm_mon], 3);

    date = std::string(buf, RFC1123_TIME_LEN);
    free(buf);
}

void highlight_crlf(const char *block, ssize_t size)
{
	std::cout << "Block with CRLF highlighted:" << std::endl << "|";
	for (ssize_t j = 0 ; j < size ; j++)
		block[j] == '\r' ? std::cout << "\\r" : (block[j] == '\n' ? std::cout << "\\n\n" : std::cout << block[j]);
	std::cout << "|" << std::endl;
}
