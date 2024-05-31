
#include "../include/configParser.hpp"

LocationBlock::LocationBlock() {}
LocationBlock::~LocationBlock() {}

ServerBlock::ServerBlock() {}
ServerBlock::~ServerBlock() {}

ConfigParser::ConfigParser(const std::string& filename) : filename(filename) {
	file.open(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Erreur lors de l'ouverture du fichier: " << filename << std::endl;
		exit(EXIT_FAILURE); // Sortie du programme avec erreur
	}
	ServerBlock server;
	server.path = filename;
}

ConfigParser::~ConfigParser() {
	file.close();
}

std::vector<ServerBlock> ConfigParser::parseConfig() {
	std::string token;
	while ((token = getNextToken()) != "") {
		if (token == "server") {
			parseServerBlock();
		}
	}
	return serverBlocks;
}

void ConfigParser::parseLocationBlock(ServerBlock& serverBlock) {
	LocationBlock locationBlock;
	std::string token;
	std::getline(file >> std::ws, locationBlock.location_path, '{'); // Récupère le chemin spécifique
	while ((token = getNextToken()) != "}") {
		if (token == "root") {
			std::getline(file >> std::ws, locationBlock.root, ';');
		} else if (token == "index") {
			std::getline(file >> std::ws, locationBlock.index, ';');
		} else if (token == "allow_methods") {
			std::getline(file >> std::ws, locationBlock.allow_methods, ';');
		} else if (token == "autoindex") {
			std::getline(file >> std::ws, locationBlock.autoindex, ';');
		} else if (token == "cgi_path") {
			std::getline(file >> std::ws, locationBlock.cgi_path, ';');
		} else if (token == "cgi_ext") {
			std::getline(file >> std::ws, locationBlock.cgi_ext, ';');
		}
	}
	serverBlock.locations.push_back(locationBlock);
}

void ConfigParser::parseServerBlock() {
	ServerBlock serverBlock;
	std::string token;
	while ((token = getNextToken()) != "}") {
		if (token == "listen") {
			file >> serverBlock.listen;
		} else if (token == "server_name") {
			std::getline(file >> std::ws, serverBlock.server_name, ';');
		} else if (token == "host") {
			std::getline(file >> std::ws, serverBlock.host, ';');
		} else if (token == "root") {
			std::getline(file >> std::ws, serverBlock.root, ';');
		} else if (token == "index") {
			std::getline(file >> std::ws, serverBlock.index, ';');
		} else if (token == "error_page") {
			std::getline(file >> std::ws, serverBlock.error_page, ';');
		} else if (token == "location") {
			parseLocationBlock(serverBlock);
		}
	}
	serverBlock.path = filename; // Ajout de l'initialisation de la variable path
	serverBlocks.push_back(serverBlock);
}

std::string ConfigParser::getNextToken() {
	std::string token;
	while (file >> token) {
		// Vérifier si le jeton commence par '{' ou ';'
		if (token[0] == '{' || token[0] == ';') {
			// Ignorer le caractère et continuer
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		// Vérifier si le jeton se termine par ';'
		if (token[token.size() - 1] == ';') {
			// Retirer le dernier caractère ';'
			token = token.substr(0, token.size() - 1);
		}
		return token;
	}
	return "";
}


int main() {
	ConfigParser parser("../config/default.conf");
	std::vector<ServerBlock> serverBlocks = parser.parseConfig();

	// Affichage des informations récupérées
	for (const auto& serverBlock : serverBlocks) {
		std::cout << "Path: " << serverBlock.path << std::endl;
		std::cout << "Listen: " << serverBlock.listen << std::endl;
		std::cout << "Server Name: " << serverBlock.server_name << std::endl;
		std::cout << "Host: " << serverBlock.host << std::endl;
		std::cout << "Root: " << serverBlock.root << std::endl;
		std::cout << "Index: " << serverBlock.index << std::endl;
		std::cout << "Error Page: " << serverBlock.error_page << std::endl;

		std::cout << "Locations:" << std::endl;
		for (const auto& location : serverBlock.locations) {
			std::cout << "\tLocation: " << location.location_path << std::endl;
			std::cout << "\t\tRoot: " << location.root << std::endl;
			std::cout << "\t\tIndex: " << location.index << std::endl;
			std::cout << "\t\tAllow Methods: " << location.allow_methods << std::endl;
			std::cout << "\t\tAutoindex: " << location.autoindex << std::endl;
			std::cout << "\t\tCGI Path: " << location.cgi_path << std::endl;
			std::cout << "\t\tCGI Ext: " << location.cgi_ext << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}
