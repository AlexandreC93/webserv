#include "../include/configParser.hpp"

// std::pair est une structure de donnees qui peut contenir deux elements
//  de types differents. Elle est definie comme ca :
// template <typename T1, typename T2>
// struct pair {
// T1 first;
// T2 second;
// };

ConfigParse::ConfigParse(const std::string &fileName)
{
	parseConfigFile(fileName);
}

ConfigParse::~ConfigParse() {};

std::vector<Server> ConfigParse::getServers() const
{
	return servers;
}

void ConfigParse::parseConfigFile(const std::string &fileName)
{
	std::ifstream configFile(fileName.c_str());
	if (!configFile.is_open())
	{
		std::cerr << "Could not open config file: " << fileName << std::endl;
		return;
	}
	std::string line;
	while(std::getline(configFile, line))
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "server")
		{
			Server server(fileName);
			while(std::getline(configFile, line))
			{
				std::istringstream serverStream(line);
				serverStream >> token;
				if (token == "{")
					break;
			}
			parseServerBlock(configFile, server);
			servers.push_back(server);
		}
	}
	configFile.close();
}

void ConfigParse::parseServerBlock(std::ifstream &ifs, Server &server)
{
	std::string line;
	while(std::getline(ifs, line))
	{
		std::istringstream serverStream(line);
		std::string token;
		serverStream >> token;

		if (token == "}")
			break;
		if (token == "listen")
		{
			std::string port;
			serverStream >> port;
			server.setListenPort(port);
		}
		else if (token == "server_name")
		{
			std::string name;
			serverStream >> name;
			server.setServerName(name);
		}
		else if (token == "host")
		{
			std::string host;
			serverStream >> host;
			server.setHost(host);
		}
		else if (token == "root")
		{
			std::string root;
			serverStream >> root;
			server.setRoot(root);
		}
		else if (token == "error_page")
		{
			std::string errorPage;
			std::vector<std::string> err_pages;
			while (serverStream >> errorPage)
			{
				err_pages.push_back(errorPage);
			}
			server.setErrPages(err_pages);
		}
		else if (token == "location")
		{
			std::string locationPath;
			serverStream >> locationPath;
			while (std::getline(ifs, line))
			{
				std::istringstream locationStream(line);
				locationStream >> token;
				if (token == "{")
					break;
			}
			parseLocationBlock(ifs, server, locationPath);
		}
	}
}

void ConfigParse::parseLocationBlock(std::ifstream &ifs, Server &server, const std::string &locationPath)
{
	Location location;
	location.setFilePath(locationPath);
	std::string line;
	while (std::getline(ifs, line))
	{
		std::istringstream locationStream(line);
		std::string token;
		locationStream >> token;

		if (token == "}")
			break;

		if (token == "root")
		{
			std::string root;
			locationStream >> root;
			location.setRoot(root, server.getRoot());
		}
		else if (token == "index")
		{
			std::vector<std::string> idxFiles;
			std::string idxFile;
			while (locationStream >> idxFile)
			{
				idxFiles.push_back(idxFile);
			}
			location.setIdxFiles(idxFiles);
		}
		// Ajoutez d'autres options de configuration pour location ici si nécessaire.
	}
	server.setLocation(locationPath, location);
}

int main()
{
    ConfigParse config("../config/default.conf");
    std::vector<Server> servers = config.getServers();

    for (size_t i = 0; i < servers.size(); ++i)
    {
        std::cout << "Server " << i + 1 << ":" << std::endl;
        std::cout << "Listen Port: " << servers[i].getListenPort() << std::endl;
        std::cout << "Host: " << servers[i].getHost() << std::endl;
        std::cout << "Server Name: " << servers[i].getServerName() << std::endl;
        std::cout << "Root: " << servers[i].getRoot() << std::endl;

        std::cout << "Error Pages: " << std::endl;
        std::map<int, std::string> errPages = servers[i].getErrPages();
        for (std::map<int, std::string>::iterator it = errPages.begin(); it != errPages.end(); ++it)
        {
            std::cout << it->first << " -> " << it->second << std::endl;
        }

        std::cout << "Locations: " << std::endl;
        std::vector<Location> locations = servers[i].getLocations();
        for (size_t j = 0; j < locations.size(); ++j)
        {
            std::cout << "Location " << j + 1 << ":" << std::endl;
            std::cout << "File Path: " << locations[j].getFilePath() << std::endl;
            std::cout << "Root: " << locations[j].getRoot() << std::endl;
            std::cout << "Index Files: ";
            std::vector<std::string> idxFiles = locations[j].getIdxFiles();
            for (size_t k = 0; k < idxFiles.size(); ++k)
            {
                std::cout << idxFiles[k] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}

// std::ostream& operator<<(std::ostream& os, const locationConfig& loc)
// {
//     os << "file_path: " << loc.file_path << ", root: " << loc.root << ", idx_files: [";
//     for (size_t i = 0; i < loc.idx_files.size(); ++i)
//     {
//         os << loc.idx_files[i];
//         if (i < loc.idx_files.size() - 1)
//         {
//             os << ", ";
//         }
//     }
//     os << "]";
//     return os;
// }

// void removeSemicolon(std::string &str)
// {
// 	while (!str.empty() && (str.back() == ';' || str.back() == '{' || str.back() == '}'))
// 	{
// 		str.pop_back();
// 	}
// }

// serverConfig parseConfig(const std::string &filename)
// {
// 	serverConfig sconfig;
// 	std::ifstream configFile(filename); // ouvre le fichier de config en lecture
// 	std::string line;					// lis chaque ligne du fichier
// 	locationConfig currentLoc;			// stock temporairement la config de l'emplacement en cours de traitement
// 	bool inLocation = false;			// indique si on est à l'intérieur d'un bloc 'location'
// 	std::string currentLocationPath;	// stock le chemin de l'emplacement actuel

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
// 			inLocation = true;
// 			std::getline(iss >> std::ws, currentLocationPath);
// 			currentLoc = locationConfig(); // Réinitialise currentLoc pour la nouvelle section location
// 			currentLoc.file_path = filename;
// 			continue; // Passe à la ligne suivante pour traiter l'intérieur du bloc location
// 		}
// 		if (key == "}" && inLocation)
// 		{
// 			inLocation = false;
// 			sconfig.locations[currentLocationPath] = currentLoc;
// 			std::cout << "Location : " << currentLocationPath << std::endl;
// 			std::cout << "File path: " << currentLoc.file_path << std::endl;
// 			continue; // Passe à la ligne suivante après avoir fermé un bloc location
// 		}
// 		if (inLocation)
// 		{
// 			if (key == "root")
// 			{
// 				iss >> currentLoc.root;
// 				removeSemicolon(currentLoc.root);
// 				std::cout << "root : " << currentLoc.root << std::endl;
// 			}
// 			else if (key == "index")
// 			{
// 				std::string index_file;
// 				// lis tous les fichiers d'index sur la ligne
// 				while (iss >> index_file)
// 				{
// 					// ajoute le fichier d'index dans la liste idx_files
// 					removeSemicolon(index_file);
// 					currentLoc.idx_files.push_back(index_file);
// 					std::cout << "Index : " << index_file << std::endl;
// 				}
// 			}
// 		}
// 		else if (key == "listen")
// 		{
// 			iss >> sconfig.listen_port;
// 			iss.ignore(1, ';');
// 		}
// 		else if (key == "server_name")
// 		{
// 			iss >> sconfig.server_name;
// 			removeSemicolon(sconfig.server_name);
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
