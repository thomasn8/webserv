#ifndef STATUSCODEEXCEPTION_HPP
# define STATUSCODEEXCEPTION_HPP

#include <exception>

class StatusCodeException : public std::exception {
	public:
		StatusCodeException(const int code) : _code(code) {}

		virtual int statuscode() const throw() {
			return _code;	
		}

	protected:
		const int _code;
};

#endif
