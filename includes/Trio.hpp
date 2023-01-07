#ifndef TRIO_HPP
# define TRIO_HPP

#include <string>

class Trio
{
	public : 
		std::string 	first;
		std::string		second;
		unsigned int	third;

		Trio();
		Trio(const Trio & src);
		Trio(const std::string & a, const std::string & b, const unsigned int & c);
		Trio & operator=(const Trio & src);
		~Trio();

		void swap(Trio & src);
};

#endif
