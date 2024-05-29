
#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <iostream>
#include <netinet/in.h>
#include "configServer.hpp"
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
#include "configServer.hpp"

class ConfigParse
{
	public:
		ConfigParse(const std::string &filename);
		~ConfigParse();

		std::vector<Server> getServers() const;

	private:
		std::vector<Server> servers;
		void parseConfigFile(const std::string &fileName);
		void parseServerBlock(std::ifstream &ifs, Server &server);
		void parseLocationBlock(std::ifstream &ifs, Server &server, const std::string &locationPath);
};

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
