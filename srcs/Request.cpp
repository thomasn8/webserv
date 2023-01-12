#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

// ---------Constructor and destructor ------------

Request::Request(std::string *rawMessage) : _rawMessage(rawMessage) {	// passe un ptr au lieu d un string pour eviter une copie
	
	// chope la 1ere ligne sans recopier toute la request
	size_t i = this->_rawMessage->find_first_of('\n');
    std::string start_line = this->_rawMessage->substr(0, i);
	
	// comme getline avance le ptr sur le stream, il faut erase le debut du string jusqu'au char suivant le \n (avance le ptr et desalloue le debut)
	_rawMessage->erase(0, i+1);
    
	// std::istringstream raw(*this->_rawMessage);
    // getline(raw, start_line);

    _check_alone_CR();					// ok
    _parse_start_line(start_line);		// optimisation possible
    _parse_header();					// reecrit en getlinant sur une string au lieu du stream (TESTER MA LOOP ET LE _split_field REECRIT AVEC DES PTR)
    _parse_body();						// en utilisant les ptr, pas besoin de recopier entierement le body
	// comme j'ai remplace le type de la variable body (par std::string *) dans la classe parent Message, tu risques d'avoir des adaptations a faire dans Responses
	
	// NBR de passe economisees: 
	// au lieu de recopier le body 3x (2x dans le constr + 1x dans parse_body), on le recopie 0x
	// header: 3x comme le body + plusieurs fois dans _parse_header()
	
	// quand on sait pas ce qu'on recoit je trouve qu'il faut tjr s'attendre au pire et optimiser a fond
	// apres pour le parse du fichier de config par exemple, j'ai aussi coder sans trop faire attention au nombre de copie de memoire vu que c'est nous qui fournissons le fichier et on connait sa taille
}

Request::Request(const Request& instance) {
    *this = instance;
}

Request::~Request() {

}

// --------- Fonctions ------------

std::string Request::get_message() const {
    return *this->_rawMessage;
}

std::string Request::get_method() const {
    return this->_method;
}

std::string Request::get_target()const {
    return this->_target;
}

std::string Request::get_version() const {
    return this->_version;
}

// RFC 9112 2.2. Message Parsing 4 paragraph about CR
void Request::_check_alone_CR() {
    std::string::iterator it;
    for (it = (*this->_rawMessage).begin(); it != (*this->_rawMessage).end(); it++) {
        if (*it == '\r' && *(it + 1) != '\n')
            *it = ' ';
    }
}

//start-line parsing
void Request::_parse_start_line(std::string startLine) {
    // std::string tmp = startLine; // pas besoin de recopier startline dans cette fonction puisque la fonction elle meme creer une copie de l'objet pour son scope
									// on peut soit passer un ptr, soit utiliser directement l'argument startline en tant qu'objet recopié
	std::string token;
    size_t pos = 0;

    startLine.erase(remove(startLine.begin(), startLine.end(), '\r'), startLine.end());
	// Dans tous les cas, startline ne contient pas le \n final donc est qu'on peut pas juste faire:
	// remove(startLine.begin(), startLine.end(), '\r'); // suffirait ?

	// ou mieux:
	
	// si le but est de tester si seulement le dernier char est un \r et, si oui, de le retirer, ca serait + efficace et suffirait de faire:
	// if (*(startLine.end()-1) == '\r')
	// 	startLine.pop_back();
    
	for(int i = 0; i < 3; i++) {
        pos = startLine.find(' ');
        if (i < 2 && pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
        if (i == 0) {
            this->_method = startLine.substr(0, pos);
            if (!(this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE"))
                throw MessageException(METHOD_NOT_ALLOWED);
        }
        else if (i == 1) {
            this->_target = startLine.substr(0, pos);
            //checker la target? (question)
        }
        else if (i == 2) {
            this->_version = startLine.substr(0, pos);
            if (this->_version.compare("HTTP/1.1") != 0) {
                throw MessageException(HTTP_VERSION_UNSUPPORTED);
            }
        }
        startLine.erase(0, pos + 1);
    }
    if (pos != std::string::npos)
        throw MessageException(BAD_REQUEST);
}

/*
	reecrit avec moins de copie de string, donc + de ptr et d'indexes
	t'as aussi oublie de trim la key je crois
*/
// void Request::_split_field(size_t pos, std::string line) {
//     std::string             key;
//     std::string             tmp;
//     std::list<std::string>  lstTmp;
//     std::string::iterator   it;
//     std::string::iterator   it2;

//     key = line.substr(0, pos);
//     line.erase(0, pos + 1);
//     while ((pos = line.find(',')) != std::string::npos) {
//         tmp = trim_sides(line.substr(0, pos));
//         lstTmp.push_back(tmp);
//         line.erase(0, pos + 1);
//     }
//     tmp = trim_sides(line.substr(0, pos));
//     lstTmp.push_back(tmp);
//     line.erase(0, pos + 1);
//     this->_fields.insert({key, lstTmp});
// }

void Request::_split_field(size_t separator, size_t lastchar) {		// A TESTER
	std::list<std::string> listValues;	
	std::string key(_rawMessage->c_str(), lastchar+1);
	const char *values = _rawMessage->c_str()+separator+1;
	const char *newvalue = values;
	int len = 0;
	while (*values)
	{
		if (*values == ',')
		{
			listValues.push_back(trim_sides(std::string(newvalue, len)));
			if (*(values+1))
				newvalue = values+1;
			else
				break;
			len = -1;
		}
		values++;
		len++;
	}
	listValues.push_back(trim_sides(std::string(newvalue, len)));
	this->_fields.insert(std::make_pair(trim_sides(key), listValues));
}

//header parsing
// void Request::_parse_header(std::istringstream &raw) {
//     std::string             line;
//     size_t                  pos = 0;

//     getline(raw, line);
//     while (!line.empty() && line != "\r") {
//         pos = line.find(':');
//         if (pos == std::string::npos)
//             throw MessageException(BAD_REQUEST);
//         _split_field(pos, line);
//         getline(raw, line);
//     }
// }

void Request::_parse_header() {
	size_t pos = 0, i = std::string::npos;
	i = this->_rawMessage->find_first_of('\n');
	while (i != std::string::npos && *this->_rawMessage != "\r")	// A TESTER, possibles erreurs dans le fonctionnement de la loop selon ce qu'on attend (j suis pas sur du format de la fin du header aussi)
	{
		// firstchar = 0, lastchar (before \n) = i-1, \n = i, len to erase = i+1
		pos = this->_rawMessage->find(':');
		if (pos == std::string::npos)
            throw MessageException(BAD_REQUEST);
		_split_field(pos, i-1);
		this->_rawMessage->erase(0, i+1);
		i = this->_rawMessage->find_first_of('\n');
	}
	if (*this->_rawMessage == "\r")
		this->_rawMessage->erase(0, i+1);	// efface la derniere ligne vide du header ??
}

//body parsing													// pas top car c'est pas vraiment un parsing mais juste une recopie de la memoire
// void Request::_parse_body(std::istringstream &raw) {			// qui peut etre lourde selon la longueur du body
//     std::string             line;

//     while (getline(raw, line)) {
//         this->_body += line;
//     }
// }

void Request::_parse_body() {									// si aucune erreur dans la maniere que j'ai reecrit les fonctions, on a plus qu'a utiliser le reste de _rawMessage pour le body
	this->_body = this->_rawMessage;
}

// --------- Operator overload ------------

Request &Request::operator=(const Request &instance) {
    this->_rawMessage = instance._rawMessage;
    this->_method = instance._method;
    this->_target = instance._target;
    this->_version = instance._version;

    this->_body = instance._body;
    this->_fields.insert(instance._fields.begin(), instance._fields.end());
    return *this;
}
