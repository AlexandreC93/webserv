
#include "../include/configLocation.hpp"

Location::Location() : file_path(""), root("") {}

Location::Location(Location const &C)
{
	*this = C;
}
Location::~Location() {}

Location &Location::operator=(Location const &C)
{
	if (this != &C)
	{
		file_path = C.getFilePath();
		root = C.getRoot();
		idx_files = C.getIdxFiles();
	}
	return *this;
}

std::string Location::getFilePath() const
{
	return file_path;
}

std::string Location::getRoot() const
{
	return root;
}

std::vector<std::string> Location::getIdxFiles()const
{
	return idx_files;
}

void Location::setFilePath(std::string file_path)
{
	this->file_path = file_path;
}

void Location::setRoot(std::string root, std::string server_root)
{
	if (root[0] == '/')
		this->root = root;
	else
		this->root = server_root + root;
}

void Location::setIdxFiles(std::vector<std::string> idx_files)
{
	this->idx_files = idx_files;
}

// int main()
// {
//     // Création d'une instance de Location
//     Location loc;

//     // Configuration des attributs
//     std::string file_path = "../config/default.conf";
//     std::string root = "./";
//     std::vector<std::string> idx_files = {"time.py", "index.html"};

//     loc.setFilePath(file_path);
//     loc.setRoot(root, "/var/www/");
//     loc.setIdxFiles(idx_files);

//     // Affichage des valeurs pour vérification
//     std::cout << "File Path: " << loc.getFilePath() << std::endl;
//     std::cout << "Root: " << loc.getRoot() << std::endl;
//     std::cout << "Index Files: ";
//     for (const std::string &file : loc.getIdxFiles())
//     {
//         std::cout << file << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }
