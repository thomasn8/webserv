#include "Trio.hpp"
#include <iostream>

Trio::Trio() :
first(std::string()), second(std::string()), third(0) 
{}

Trio::Trio(const Trio & src) :
first(src.first), second(src.second), third(src.third)
{}

Trio::Trio(const std::string & a, const std::string & b, const unsigned int & c) :
first(a), second(b), third(c)
{}

Trio & Trio::operator=(const Trio & src) 
{ 
	first = src.first;
	second = src.second;
	third = src.third;
	return *this;
}

Trio::~Trio() {}

void Trio::swap(Trio & src)
{
	Trio tmp = src;
	src.first = this->first;
	src.second = this->second;
	src.third = this->third;
	this->first = tmp.first;
	this->second = tmp.second;
	this->third = tmp.third;
}

/* 
	************ NON MEMBER FUNCTION
*/
Trio make_trio(std::string x, std::string y, int z)
{
	return (Trio(x,y,z));
}

void swap(Trio & x, Trio & y)
{
	x.swap(y); 
}

bool operator==(const Trio & lhs, const Trio & rhs) 
{
	return lhs.first == rhs.first && lhs.second == rhs.second && lhs.third == rhs.third; 
}

bool operator!=(const Trio & lhs, const Trio & rhs)
{
	return !(lhs == rhs); 
}
