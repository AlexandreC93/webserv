
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
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

struct locationConfig
{
	std::string path; //chemin de l'emplacement
	std::string root; //racine de l'emplacement
	std::vector<std::string> idx_files; //liste de fichiers d'index pour l'emplacement
};

struct serverConfig
{
	int listen_port; //port d'ecoute du server
	std::string server_name; //nom du server
	//err_pages est une std::map qui associe des clés de type int à des valeurs de type std::string :
	std::map<int, std::string> err_pages; //map associant des codes d'erreur a des pages d'erreur
	std::map<std::string, locationConfig> locations; // map associant des chemins d'emplacement a des connfig d'emplacement
};

serverConfig parseConfig(const std::string& filename);

#endif
