#ifndef STATUSCODEEXCEPTION_HPP
# define STATUSCODEEXCEPTION_HPP

#include <exception>
#include <string>

#include "methods.hpp"

class StatusCodeException : public std::exception {
	public:
		StatusCodeException(const int code) : _code(code) {}

		virtual const int what() throw() {
			return _code;	
		}

	protected:
		const int _code;
};

#endif
