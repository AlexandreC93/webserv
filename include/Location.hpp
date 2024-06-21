#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

//chaque server a son propre vecteur de location qui lui correspond

class Location // chaque bloc location est contenu dans un vecteur dans server
{
public:
	Location();
	Location(Location const &src);
	~Location();
	// void parseLocationConfig(const std::string &configBlock);

private:
	std::string _path;
	std::string _root;
	std::string _index;
	bool _autoindex;
	std::vector<std::string> _methods;
	bool _cgiLocation;
	std::map<std::string, std::string> _cgiExtensionAndPath;
	int _redirectionCode;
	std::string _redirection;
	std::string _uploadDir;
	static bool _verbose;
};

#endif

// std::string path;
// std::string root;
// std::vector<std::string> index_files;
// std::string cgi_path;
// std::vector<std::string> methods;
