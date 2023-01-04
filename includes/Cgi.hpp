#ifndef CGI_HPP
# define CGI_HPP

class Cgi {
	public:
		//Cgi();
		Cgi(const Cgi &instance);
		virtual ~Cgi();

		Cgi		&operator=(const Cgi &instance);

	private:

};

#endif