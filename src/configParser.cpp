#include "../include/configParser.hpp"

// std::pair est une structure de donnees qui peut contenir deux elements
//  de types differents. Elle est definie comme ca :
// template <typename T1, typename T2>
// struct pair {
// T1 first;
// T2 second;
// };

std::ostream& operator<<(std::ostream& os, const locationConfig& loc)
{
    os << "file_path: " << loc.file_path << ", root: " << loc.root << ", idx_files: [";
    for (size_t i = 0; i < loc.idx_files.size(); ++i)
    {
        os << loc.idx_files[i];
        if (i < loc.idx_files.size() - 1)
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

void removeSemicolon(std::string &str)
{
	while (!str.empty() && (str.back() == ';' || str.back() == '{' || str.back() == '}'))
	{
		str.pop_back();
	}
}

serverConfig parseConfig(const std::string &filename)
{
	serverConfig sconfig;
	std::ifstream configFile(filename); // ouvre le fichier de config en lecture
	std::string line;					// lis chaque ligne du fichier
	locationConfig currentLoc;			// stock temporairement la config de l'emplacement en cours de traitement
	bool inLocation = false;			// indique si on est à l'intérieur d'un bloc 'location'
	std::string currentLocationPath;	// stock le chemin de l'emplacement actuel

	while (std::getline(configFile, line)) // lis le fichier ligne par ligne
	{
		std::istringstream iss(line); // creer un flux de chaine a partir de la ligne lue
		std::string key;			  // stock le 1er mot de la ligne, qui est une clef
		iss >> key;					  // lit la clef depuis le flux
		// ignore les lignes contenant server ou location
		if (key == "server")
			continue;
		// stock les keys dans les variables
		if (key == "location")
		{
			inLocation = true;
			std::getline(iss >> std::ws, currentLocationPath);
			currentLoc = locationConfig(); // Réinitialise currentLoc pour la nouvelle section location
			currentLoc.file_path = filename;
			continue; // Passe à la ligne suivante pour traiter l'intérieur du bloc location
		}
		if (key == "}" && inLocation)
		{
			inLocation = false;
			sconfig.locations[currentLocationPath] = currentLoc;
			std::cout << "Location : " << currentLocationPath << std::endl;
			std::cout << "File path: " << currentLoc.file_path << std::endl;
			continue; // Passe à la ligne suivante après avoir fermé un bloc location
		}
		if (inLocation)
		{
			if (key == "root")
			{
				iss >> currentLoc.root;
				removeSemicolon(currentLoc.root);
				std::cout << "root : " << currentLoc.root << std::endl;
			}
			else if (key == "index")
			{
				std::string index_file;
				// lis tous les fichiers d'index sur la ligne
				while (iss >> index_file)
				{
					// ajoute le fichier d'index dans la liste idx_files
					removeSemicolon(index_file);
					currentLoc.idx_files.push_back(index_file);
					std::cout << "Index : " << index_file << std::endl;
				}
			}
		}
		else if (key == "listen")
		{
			iss >> sconfig.listen_port;
			iss.ignore(1, ';');
		}
		else if (key == "server_name")
		{
			iss >> sconfig.server_name;
			removeSemicolon(sconfig.server_name);
		}
		else if (key == "error_page")
		{
			std::string error_page;
			while (iss >> error_page)
			{
				std::stringstream ss(error_page);
				int error_code;
				if (ss >> error_code)
				{
					sconfig.err_pages[error_code] = error_page;
					removeSemicolon(sconfig.err_pages[error_code]);
					std::cout << "Err_page : " << sconfig.err_pages[error_code] << std::endl;
				}
			}
		}
	}
	return sconfig;
}
