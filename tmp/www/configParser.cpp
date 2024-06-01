#include "../include/configParser.hpp"

//std::pair est une structure de donnees qui peut contenir deux elements
// de types differents. Elle est definie comme ca :
//template <typename T1, typename T2>
//struct pair {
//	T1 first;
//	T2 second;
//};


serverConfig parseConfig(const std::string& filename)
{
	serverConfig serverConfig;
	std::ifstream configFile(filename); //ouvre le fichier de config en lecture
	std::string line; //lis chaque ligne du fichier
	locationConfig currentLoc; //stock temporairement la config de l'emplacement en cours de traitement
	bool inLocation = false; //indique si on est a l'interieur d'un bloc 'location'

	while(std::getline(configFile, line)) //lis le fichier ligne par ligne
	{
		std::istringstream iss(line); //creer un flux de chaine a partir de la ligne lue
		std::string key; // stock le 1er mot de la ligne, qui est une clef
		iss >> key; // lit la clef depuis le flux
		//ignore les lignes contenant server ou location
		if (key == "server" || key == "location")
			continue ;
		//stock les keys dans les variables
		if (key == "listen")
		{
			iss >> serverConfig.listen_port;
			iss.ignore(1, ';');
		}
		else if (key == "server_name")
		{
			iss >> serverConfig.server_name;
			iss.ignore(1, ';');
		}
		else if (key == "root")
		{
			//verifie si on est dans un bloc 'location'
			if (inLocation)
				iss >> currentLoc.root;
			else
				std::cerr << "Error: root must be inside location block" << std::endl;
			iss.ignore(1, ';');
		}
		else if (key == "index")
		{
			std::string index_file;
			//lis tous les fichiers d'index sur la ligne
			while(iss >> index_file)
			{
				//ajoute le fichier d'index dans la liste idx_files
				currentLoc.idx_files.push_back(index_file);
			}
			iss.ignore(1, ';');
		}
		else if ("error_page")
		{
			int error_code;
			std::string error_page;
			while(iss >> error_code)
			{
				//ajoute le code d'erreur a la map 'err_pages' avec une chaine vide comme valeur initiale
				serverConfig.err_pages[error_code] = "";
			}
			iss >> error_page;
			iss.ignore(1, ';');
			// auto permet de deduire automatiquement le type de 'code'
			//code est une ref (&) a un element de la map
			for (auto& code : serverConfig.err_pages)
			{
				code.second = error_page;
			}
		}
		else if (key == "}")
		{
			if (inLocation)
			{
				//ajoute 'currentLoc' a serverConfig.locations avec le chemin comme clef
				serverConfig.locations[currentLoc.path] = currentLoc;
				//reinitialise 'currentLoc' et marque la fin du bloc 'location'
				currentLoc = locationConfig();
				inLocation = false;
			}
		}
		else if (key == "=")
		{
			//lit le chemin de l'emplacement et marque le debut d'un nouveau bloc 'locations'
			iss >> currentLoc.path;
			inLocation = true;
		}
	}
	return serverConfig;
}
