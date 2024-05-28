
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <netinet/in.h>
// # include <fstream>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
// # include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <cstdarg>
// #include containers
#include <vector>
#include <map>

class Location
{
	public:
		Location();
		Location(Location const &C);
		~Location();

		Location &operator=(Location const &C);

	private:
		std::string file_path;
		std::string root;
		std::vector<std::string> idx_files;
};

class Server
{
	public:
		Server();
		Server(Server const &C);
		~Server();

		Server &operator=(Server const &C);

	private:
		in_port_t listen_port;
		in_addr_t host;
		std::string server_name;
		std::map<int, std::string> err_pages;
		std::vector<Location> locations;
};

struct locationConfig
{
	std::string file_path; //chemin de l'emplacement
	std::string root; //racine de l'emplacement
	std::vector<std::string> idx_files; //liste de fichiers d'index pour l'emplacement
};

struct serverConfig
{
	int listen_port; //port d'ecoute du server
	std::string server_name; //nom du server
	//err_pages est une std::map qui associe des clés de type int à des valeurs de type std::string :
	std::map<int, std::string> err_pages; //map associant des codes d'erreur a des pages d'erreur
	std::map<std::string, locationConfig> locations; // map associant des chemins d'emplacement a des config d'emplacement
};

void	removeSemicolon(std::string &str);
serverConfig parseConfig(const std::string& filename);

std::ostream& operator<<(std::ostream& os, const locationConfig& loc);

#endif





// serverConfig parseConfig(const std::string &filename)
// {
// 	serverConfig sconfig;
// 	std::ifstream configFile(filename); // ouvre le fichier de config en lecture
// 	std::string line;					// lis chaque ligne du fichier
// 	locationConfig currentLoc;			// stock temporairement la config de l'emplacement en cours de traitement
// 	// bool inLocation = true;			// indique si on est a l'interieur d'un bloc 'location'

// 	while (std::getline(configFile, line)) // lis le fichier ligne par ligne
// 	{
// 		std::istringstream iss(line); // creer un flux de chaine a partir de la ligne lue
// 		std::string key;			  // stock le 1er mot de la ligne, qui est une clef
// 		iss >> key;					  // lit la clef depuis le flux
// 		// ignore les lignes contenant server ou location
// 		if (key == "server")
// 			continue;
// 		// stock les keys dans les variables
// 		if (key == "location")
// 		{
// 			std::string location_path;
// 			std::getline(iss >> std::ws, location_path);
// 			// Affectation des valeurs appropriées à la nouvelle instance de locationConfig
// 			currentLoc.file_path = "../config/default.conf";
// 			sconfig.locations[location_path] = location_path;

// 			std::cout << "Location : " << sconfig.locations[location_path] << std::endl;
// 			std::cout << "File path : " << currentLoc.file_path << std::endl;
// 		}
// 		if (key == "listen")
// 		{
// 			iss >> sconfig.listen_port;
// 			iss.ignore(1, ';');
// 		}
// 		else if (key == "server_name")
// 		{
// 			iss >> sconfig.server_name;
// 			removeSemicolon(sconfig.server_name);
// 		}
// 		else if (key == "root")
// 		{
// 			// verifie si on est dans un bloc 'location'
// 			iss >> currentLoc.root;
// 			removeSemicolon(currentLoc.root);
// 			std::cout << "root : " << currentLoc.root << std::endl;
// 		}
// 		else if (key == "index")
// 		{
// 			std::string index_file;
// 			// lis tous les fichiers d'index sur la ligne
// 			while (iss >> index_file)
// 			{
// 				// ajoute le fichier d'index dans la liste idx_files
// 				currentLoc.idx_files.push_back(index_file);
// 				removeSemicolon(index_file);
// 				std::cout << "Index : " << index_file << std::endl;
// 			}
// 		}
// 		else if (key == "error_page")
// 		{
// 			std::string error_page;
// 			while (iss >> error_page)
// 			{
// 				std::stringstream ss(error_page);
// 				int error_code;
// 				if (ss >> error_code)
// 				{
// 					sconfig.err_pages[error_code] = error_page;
// 					removeSemicolon(sconfig.err_pages[error_code]);
// 					std::cout << "Err_page : " << sconfig.err_pages[error_code] << std::endl;
// 				}
// 			}
// 		}
// 	}
// 	return sconfig;
// }
