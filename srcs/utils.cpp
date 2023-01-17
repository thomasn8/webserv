#include "../includes/utils.hpp"

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

char *Rfc1123_DateTimeNow()
{
    const int RFC1123_TIME_LEN = 29;
    time_t t;
    struct tm tm;
    char * buf = (char *)malloc(RFC1123_TIME_LEN+1);

    time(&t);
    gmtime_r(&t, &tm);

    strftime(buf, RFC1123_TIME_LEN+1, "---, %d --- %Y %H:%M:%S GMT", &tm);
    memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
    memcpy(buf+8, MONTH_NAMES[tm.tm_mon], 3);

    return buf;
}

void highlight_crlf(const char *block, ssize_t size)
{
	std::cout << "Block with CRLF highlighted:" << std::endl << "|";
	for (ssize_t j = 0 ; j < size ; j++)
		block[j] == '\r' ? std::cout << "\\r" : (block[j] == '\n' ? std::cout << "\\n\n" : std::cout << block[j]);
	std::cout << "|" << std::endl;
}
