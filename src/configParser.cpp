
#include "../include/configParser.hpp"

ConfigParser::ConfigParser(const std::string path) : _path(path) {
	std::cout << "PATH:" << path << std::endl;
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
    std::ifstream configFile(_path.c_str());
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

void ConfigParser::mainParse() {
    for (std::vector<ServerBlock>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
        const ServerBlock &serverBlock = *it;
        std::cout << "Chemin : " << serverBlock.getPath() << std::endl;
        std::cout << "Écoute : " << serverBlock.getListen() << std::endl;
        std::cout << "Nom du serveur : " << serverBlock.getServerName() << std::endl;
        std::cout << "Hôte : " << serverBlock.getHost() << std::endl;
        std::cout << "Racine : " << serverBlock.getRoot() << std::endl;
        std::cout << "Index : " << serverBlock.getIndex() << std::endl;

        const std::map<int, std::string> &pagesErreurs = serverBlock.getErrorPage();
        for (std::map<int, std::string>::const_iterator errIt = pagesErreurs.begin(); errIt != pagesErreurs.end(); ++errIt) {
            std::cout << "\t" << errIt->first << ": " << errIt->second << std::endl;
        }

        std::cout << "Taille maximale du corps client : " << serverBlock.getClientMaxBodySize() << std::endl;
        std::cout << "Serveur par défaut : " << serverBlock.getDefaultServer() << std::endl;

        const std::vector<LocationBlock> &emplacements = serverBlock.getLocations();
        for (std::vector<LocationBlock>::const_iterator locIt = emplacements.begin(); locIt != emplacements.end(); ++locIt) {
            const LocationBlock &location = *locIt;
            std::cout << "\tEmplacement : " << location.getLocationPath() << std::endl;
            std::cout << "\t\tRacine : " << location.getRoot() << std::endl;
            std::cout << "\t\tIndex : " << location.getIndex() << std::endl;
            std::cout << "\t\tMéthodes autorisées : " << location.getAllowMethods() << std::endl;
            std::cout << "\t\tAutoindex : " << location.getAutoIndex() << std::endl;
            std::cout << "\t\tStockage d'upload : " << location.getUploadStore() << std::endl;
            std::cout << "\t\tChemin CGI : " << location.getCgiPath() << std::endl;
            std::cout << "\t\tExtension CGI : " << location.getCgiExt() << std::endl;
            std::cout << "\t\tCGI : " << location.getCgi() << std::endl;
            std::cout << "\t\tRetour : " << location.getRet() << std::endl;
        }
    }
}


