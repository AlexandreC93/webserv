
#include "../include/configParser.hpp"

ConfigParser::ConfigParser(const std::string path) : _path(path) {
	std::cout << path << std::endl;
	_path = path;
}

ConfigParser::ConfigParser(ConfigParser const &src){
		if (this != &src)
		*this = src;
}

ConfigParser::~ConfigParser() {
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

std::string ConfigParser::getPath() const 
{
    return (_path);
}

std::vector<ServerBlock> ConfigParser::getServers() const 
{
    return (_servers);
}

std::vector<std::string>    ConfigParser::getServerConf() const 
{
    return (_serverConf);
}

int ConfigParser::getServerNb() const
{
    return (_serverNb);
}

std::string ConfigParser::extractContent(std::string const &path) 
{
    struct stat buf;
    int ret = stat(path.c_str(), &buf);
    std::ifstream fileStream;

    if (ret < 0 || !S_ISREG(buf.st_mode))
        throw ConfParserException("Invalid path to configuration file");
    if (access(path.c_str(), R_OK) != 0)
        throw ConfParserException("No reading rights on configuration file");
    fileStream.open(path.c_str());
    if (fileStream.fail())
        throw ConfParserException("Configuration file could not be opened");

    std::stringstream result;
    result << fileStream.rdbuf();
    fileStream.close();
    return (result.str());
}



std::string ConfigParser::cleanFile(std::string &content) const {
    // Suppression des commentaires
    size_t sharp = content.find('#');
    while (sharp != std::string::npos) {
        size_t nl = content.find('\n', sharp);
        content.erase(sharp, nl - sharp);
        sharp = content.find('#');
    }

    return content;
}



std::vector<ServerBlock> ConfigParser::parseConfig() {
    std::ifstream configFile(_path);
    if (!configFile)
        throw std::runtime_error("Cannot open configuration file.");

    // Lire tout le contenu du fichier
    std::stringstream buffer;
    buffer << configFile.rdbuf();
    std::string content = buffer.str();

    // Nettoyer le contenu (suppression des commentaires uniquement)
    std::string cleanedContent = cleanFile(content);

    // Créer un stringstream à partir du contenu nettoyé
    std::istringstream cleanedStream(cleanedContent);

    std::string token;
    while (cleanedStream >> token) {
        if (token == "server") {
            ServerBlock serverBlock;
            serverBlock.parseServerBlock(cleanedStream);
            _servers.push_back(serverBlock);
        }
        // Autres traitements selon les tokens lus
    }

    return _servers;
}





// std::vector<ServerBlock> ConfigParser::parseConfig() {
// 	std::string token;
// 	while ((token = cleanFile()) != "") {
// 		if (token == "server") {
// 			parseServerBlock();
// 		}
// 	}
// 	return serverBlock;
// }

//AJOUTER COMMENTAIRES A RETIRER-EVITER
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

void ConfigParser::mainParse(void)
{
    // Affichage des informations récupérées
    for (const auto& serverBlock : _servers) {
        std::cout << "Path: " << serverBlock.getPath() << std::endl;
        std::cout << "Listen: " << serverBlock.getListen() << std::endl;
        std::cout << "Server Name: " << serverBlock.getServerName() << std::endl;
        std::cout << "Host: " << serverBlock.getHost() << std::endl;
        std::cout << "Root: " << serverBlock.getRoot() << std::endl;
        std::cout << "Index: " << serverBlock.getIndex() << std::endl;
        std::cout << "Error Pages:" << std::endl;
        for (const auto& error : serverBlock.getErrorPage()) {
            std::cout << "\t" << error.first << ": " << error.second << std::endl;
        }
        std::cout << "Client size: " << serverBlock.getClientMaxBodySize() << std::endl;
        std::cout << "Default server: " << serverBlock.getDefaultServer() << std::endl;

        std::cout << "Locations:" << std::endl;
        for (const auto& location : serverBlock.getLocations()) {
            std::cout << "\tLocation: " << location.getLocationPath() << std::endl;
            std::cout << "\t\tRoot: " << location.getRoot() << std::endl;
            std::cout << "\t\tIndex: " << location.getIndex() << std::endl;
            // Afficher d'autres informations pertinentes pour LocationBlock
        }
        std::cout << std::endl;
    }
}


