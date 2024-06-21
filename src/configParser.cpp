
#include "../include/configParser.hpp"

// LocationBlock::LocationBlock() {}
// LocationBlock::~LocationBlock() {} //mettre la class location dans le fichier location.hpp

// ServerBlock::ServerBlock() {}
// ServerBlock::~ServerBlock() {} // mettre la class location dans le fichier serverBlock.hpp

// ConfigParser::ConfigParser(const std::string& filename) : filename(filename) { //parser le fichier dans parseconfig pour trouver si y'a des erreurs global et ensuite recuperer les different elements dans les class correspondante
// 	file.open(filename);
// 	std::cout << filename << std::endl;
// 	if (!file.is_open()) {
// 		std::cerr << "Erreur lors de l'ouverture du fichier: " << filename << std::endl;
// 		exit(EXIT_FAILURE); // Sortie du programme avec erreur
// 	}
// 	ServerBlock server;
// 	server.path = filename;
// }

ConfigParser::ConfigParser(std::string path) : _path(path)
{
	_serverNb = 0;
}

ConfigParser::ConfigParser(ConfigParser const &src)
{
	if (this != &src)
		*this = src;
}

ConfigParser::~ConfigParser() {
	getPath();
}

ConfigParser &ConfigParser::operator=(ConfigParser const &rhs)
{
	if (this != &rhs)
	{
		_path = rhs.getPath();
		_servers = rhs.getServers();
		_serverConf = rhs.getServerConf();
		_serverNb = rhs.getServerNb();
	}
	return (*this);
}

std::string ConfigParser::getPath() const{
	return _path;
}

std::vector<Server> ConfigParser::getServers() const{
	return _servers;
}

std::vector<std::string> ConfigParser::getServerConf() const{
	return _serverConf;
}

int ConfigParser::getServerNb() const{
	return _serverNb;
}

void ConfigParser::parse(void){

}

std::string ConfigParser::cleanConf(std::istringstream &file) const
{
	std::string token;
	while (file >> token)
	{
		//Verifier si le jeton commence par '#' (debut commentaire)
		if (token[0] == '#')
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		//Verifier si le jeton commence par '{' ou ';'
		if (token[0] == '{' || token[0] == ';')
		{
			//Ignorer le caractere et continuer
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		//Verifier si le jeton se termine par ';'
		if (token[token.size() - 1] == ';')
		{
			//Retirer le dernier caractere ';'
			token = token.substr(0, token.size() - 1);
		}
		return token;
	}
	//Si aucun jeton valide n'est trouve, retourner une chaine vide
	return "";
}

std::pair<size_t, size_t> ConfigParser::findServerBlocks(size_t blockStart, const std::string &content) const
{
	size_t i = blockStart;
	int subContext = 0;
	// Trouver le début du bloc de configuration
	while (content[i] != '\0')
	{
		if (std::isspace(content[i]))
		{
			i++;
			continue;
		}
		else if (content[i] == 's' && content.compare(i, 6, "server") == 0)
		{
			i += 6;
			while (content[i] != '\0' && std::isspace(content[i]))
				i++;
			if (content[i] == '{')
				break;
			else
				throw ConfParserException("Expected '{' symbol after server directive");
		}
		else
			throw ConfParserException("Unknown directive or unexpected symbol in main context");
	}
	size_t blockStartIndex = i;
	// Trouver la fin du bloc de configuration
	for (i = blockStartIndex + 1; content[i] != '\0'; i++)
	{
		if (content[i] == '{')
			subContext++;
		else if (content[i] == '}' && subContext > 0)
			subContext--;
		else if (content[i] == '}' && subContext == 0)
			return std::make_pair(blockStartIndex, i);
	}
	//renvoi un std::pair contenant les indices de debut et de fin d'un bloc de serveur.
	return std::make_pair(blockStartIndex, blockStartIndex); // En cas d'erreur, retourner le début
}

void ConfigParser::splitServerBlocks(std::string &content)
{
	size_t blockStart = 0;
	size_t blockEnd = 1;
	if (content.find("server") == std::string::npos)
		throw ConfParserException("No server block found in configuration file");
	while (blockStart != blockEnd && blockStart < content.length())
	{
		std::pair<size_t, size_t> blockIdx = findServerBlocks(blockStart, content);
		blockStart = blockIdx.first;
		blockEnd = blockIdx.second;
		if (blockStart == blockEnd)
		{
			for (size_t i = blockStart; content[i] != '\0'; i++)
			{
				if (!std::isspace(content[i]))  // en cas de whitespaces en fin de fichier
					throw ConfParserException("Something seems wrong with curved brackets scopes");
			}
			break;
		}
		_serverConf.push_back(content.substr(blockStart, blockEnd - blockStart + 1));
		_serverNb++;
		blockStart = blockEnd + 1;
	}
}

// std::vector<ServerBlock> ConfigParser::parseConfig() { //split les server dans server block avant de parser chaque server
// 	std::string token;
// 	while ((token = getNextToken()) != "") {
// 		if (token == "server") {
// 			parseServerBlock();
// 		}
// 	}
// 	return serverBlocks;
// }

// void ConfigParser::parseLocationBlock(ServerBlock& serverBlock) {
// 	LocationBlock locationBlock;
// 	std::string token;
// 	std::getline(file >> std::ws, locationBlock.location_path, '{'); // Récupère le chemin spécifique
// 	while ((token = getNextToken()) != "}") {
// 		if (token == "root") {
// 			std::getline(file >> std::ws, locationBlock.root, ';');
// 		} else if (token == "index") {
// 			std::getline(file >> std::ws, locationBlock.index, ';');
// 		} else if (token == "allow_methods" || token == "methods") {
// 			std::getline(file >> std::ws, locationBlock.allow_methods, ';');
// 		} else if (token == "autoindex") {
// 			std::getline(file >> std::ws, locationBlock.autoindex, ';');
// 		}else if (token == "upload_store"){
// 			std::getline(file >> std::ws, locationBlock.upload_store, ';');
// 		} else if (token == "cgi_path") {
// 			std::getline(file >> std::ws, locationBlock.cgi_path, ';');
// 		} else if (token == "cgi_ext") {
// 			std::getline(file >> std::ws, locationBlock.cgi_ext, ';');
// 		} else if (token == "cgi"){
// 			std::getline(file >> std::ws, locationBlock.cgi, ';');
// 		} else if (token == "return")
// 		{
// 			std::getline(file >> std::ws, locationBlock.ret, ';');
// 		}
// 	}
// 	serverBlock.locations.push_back(locationBlock);
// }

// void ConfigParser::parseServerBlock()
// {
// 	ServerBlock serverBlock;
// 	std::string token;
// 	while ((token = getNextToken()) != "}") {
// 		if (token == "listen") {
// 			file >> serverBlock.listen;
// 		} else if (token == "server_name") {
// 			std::getline(file >> std::ws, serverBlock.server_name, ';');
// 		} else if (token == "host") {
// 			std::getline(file >> std::ws, serverBlock.host, ';');
// 		} else if (token == "root") {
// 			std::getline(file >> std::ws, serverBlock.root, ';');
// 		} else if (token == "index") {
// 			std::getline(file >> std::ws, serverBlock.index, ';');
// 		} else if (token == "client_max_body_size") {
// 			std::getline(file >> std::ws, serverBlock.client_max_body_size, ';');
// 		} else if (token == "default_server") {
// 			std::getline(file >> std::ws, serverBlock.default_server, ';');
// 		} else if (token == "error_page") {
// 			std::string error_info;
// 			std::getline(file >> std::ws, error_info, ';');

// 			std::istringstream error_stream(error_info);
// 			int error_code;
// 			std::string error_path;
// 			error_stream >> error_code;
// 			error_stream >> error_path;
// 			serverBlock.error_page[error_code] = error_path;
// 		} else if (token == "location") {
// 			parseLocationBlock(serverBlock);
// 		}
// 	}
// 	serverBlock.path = filename; // Ajout de l'initialisation de la variable path
// 	serverBlocks.push_back(serverBlock);
// }

// std::string ConfigParser::getNextToken() {
// 	std::string token;
// 	while (file >> token) {
// 		// Vérifier si le jeton commence par '{' ou ';'
// 		if (token[0] == '{' || token[0] == ';') {
// 			// Ignorer le caractère et continuer
// 			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// 			continue;
// 		}
// 		// Vérifier si le jeton se termine par ';'
// 		if (token[token.size() - 1] == ';') {
// 			// Retirer le dernier caractère ';'
// 			token = token.substr(0, token.size() - 1);
// 		}
// 		return token;
// 	}
// 	return "";
// }

// std::vector<ServerBlock> ConfigParser::getServerBlocks() const
// {
//     return serverBlocks;
// }

void ConfigParser::mainParse(void) {

	// Affichage des informations récupérées
	for (const auto& serverBlock : serverBlocks) {
		std::cout << "Path: " << serverBlock.path << std::endl;
		std::cout << "Listen: " << serverBlock.listen << std::endl;
		std::cout << "Server Name: " << serverBlock.server_name << std::endl;
		std::cout << "Host: " << serverBlock.host << std::endl;
		std::cout << "Root: " << serverBlock.root << std::endl;
		std::cout << "Index: " << serverBlock.index << std::endl;
		std::cout << "Error Pages:" << std::endl;
		for (std::map<int, std::string>::const_iterator it_err = serverBlock.error_page.begin(); it_err != serverBlock.error_page.end(); ++it_err) {
			std::cout << "\t" << it_err->first << ": " << it_err->second << std::endl;
		}
		std::cout << "Client size: " << serverBlock.client_max_body_size << std::endl;
		std::cout << "Default server: " << serverBlock.default_server << std::endl;

		std::cout << "Locations:" << std::endl;
		for (const auto& location : serverBlock.locations) {
			std::cout << "\tLocation: " << location.location_path << std::endl;
			std::cout << "\t\tRoot: " << location.root << std::endl;
			std::cout << "\t\tIndex: " << location.index << std::endl;
			std::cout << "\t\tAllow Methods: " << location.allow_methods << std::endl;
			std::cout << "\t\tAutoindex: " << location.autoindex << std::endl;
			std::cout << "\t\tUpload_store: " << location.upload_store << std::endl;
			std::cout << "\t\tCGI Path: " << location.cgi_path << std::endl;
			std::cout << "\t\tCGI Ext: " << location.cgi_ext << std::endl;
			std::cout << "\t\tCGI: " << location.cgi << std::endl;
			std::cout << "\t\tReturn: " << location.ret << std::endl;
		}
		std::cout << std::endl;
	}
}
