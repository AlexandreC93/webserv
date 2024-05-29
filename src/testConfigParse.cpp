#include <iostream>
#include <cassert>
#include "../include/configParser.hpp"

int main() {
    std::string filename = "../config/default.conf";
    ServerConfig serverConfig = parseConfig(filename);

    // Print server configuration for verification
    std::cout << "listen_port: " << serverConfig.listen_port << std::endl;
    std::cout << "server_name: " << serverConfig.server_name << std::endl;
    std::cout << "Error pages: " << std::endl;
    for (std::map<int, std::string>::iterator it = serverConfig.err_pages.begin(); it != serverConfig.err_pages.end(); ++it) {
        std::cout << "Error code: " << it->first << ", Page: " << it->second << std::endl;
    }
    std::cout << "Locations: " << std::endl;
    for (std::map<std::string, LocationConfig>::iterator it = serverConfig.locations.begin(); it != serverConfig.locations.end(); ++it) {
        std::cout << "Path: " << it->first << std::endl;
        std::cout << it->second << std::endl;
    }

    return 0;
}



// Fonction de test pour vérifier les valeurs de configuration
// void testConfigParser() {
//     // Parse the example configuration file
//     serverConfig config = parseConfig("../config/default.conf");

//     // Affichage des valeurs pour vérifier le bon fonctionnement du parseur
//     std::cout << "listen_port: " << config.listen_port << std::endl;
//     std::cout << "server_name: " << config.server_name << std::endl;

//     // Parcourir les emplacements (locations) dans la configuration
//     std::map<std::string, locationConfig>::const_iterator it;
//     for (it = config.locations.begin(); it != config.locations.end(); ++it) {
// 		// std::cout << "Location path: " << it->second.path << std::endl;
//         std::cout << "Root: " << it->second.root << std::endl;
//         std::cout << "Index files: ";

//         // Parcourir les fichiers d'index
//         std::vector<std::string>::const_iterator file_it;
//         for (file_it = it->second.idx_files.begin(); file_it != it->second.idx_files.end(); ++file_it) {
//             std::cout << *file_it << " ";
//         }
//         std::cout << std::endl;
//     }

    // Affichage des pages d'erreur
    // std::cout << "Error pages: " << std::endl;
    // std::map<int, std::string>::const_iterator err_it;
    // for (err_it = config.err_pages.begin(); err_it != config.err_pages.end(); ++err_it) {
    //     std::cout << "Error code: " << err_it->first << ", Page: " << err_it->second << std::endl;
    // }
// }

// int main() {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//         std::string configFilePath = std::string(cwd) + "/config/default.conf";
//         std::cout << "Config file path: " << configFilePath << std::endl;
//         // Maintenant, vous pouvez utiliser configFilePath pour passer à parseConfig
//     } else {
//         std::cerr << "Error: Could not get current working directory" << std::endl;
//         return 1;
//     }
//     return 0;
// }

// int main() {
//     testConfigParser();
//     return 0;
// }
