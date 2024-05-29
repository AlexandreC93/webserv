#include "../include/configServer.hpp"

Server::Server(const std::string &configFile)
{
	listen_port = 0;
	host = inet_addr("127.0.0.1"); // Valeur par défaut
	server_name = "";
	root = "";
	err_pages.clear();
}

Server::Server(Server const &C)
{
	*this = C;
}

Server::~Server() {};

Server &Server::operator=(Server const &C)
{
	if (this != &C)
	{
		listen_port = C.getListenPort();
		host = C.getHost();
		server_name = C.getServerName();
		root = C.getRoot();
		err_pages = C.getErrPages();
		locations = C.getLocations();
	}
	return *this;
}

in_port_t Server::getListenPort() const
{
	return listen_port;
}

in_addr_t Server::getHost() const
{
	return host;
}

std::string Server::getServerName() const
{
	return server_name;
}

std::string Server::getRoot() const
{
	return root;
}

std::map<int, std::string> Server:: getErrPages() const
{
	return err_pages;
}

std::vector<Location> Server::getLocations() const
{
	return locations;
}

void Server::setListenPort(std::string listen_port)
{
	this->listen_port = static_cast<in_port_t>(std::stoi(listen_port));
}

void Server::setHost(std::string host)
{
	this->host = inet_addr(host.c_str());
}

void Server::setServerName(std::string server_name)
{
	this->server_name = server_name;
}

void Server::setRoot(std::string root)
{
	this->root = root;
}

void Server::setErrPages(std::vector<std::string> err_pages)
{
	for (const std::string &error_page : err_pages)
	{
		std::istringstream iss(error_page);
		int code;
		std::string page;
		iss >> code >> page;
		this->err_pages[code] = page;
	}
}

void Server::setLocation(std::string path, Location location)
{
	locations.push_back(location);
}


// int main()
// {
//     // Création d'une instance de Server
//     Server server("default.conf");

//     // Configuration des attributs
//     server.setListenPort("8002");
//     server.setHost("127.0.0.1");
//     server.setServerName("localhost");
//     server.setRoot("docs/fusion_web/");

//     std::vector<std::string> err_pages = {"404 error_pages/404.html"};
//     server.setErrPages(err_pages);

//     // Création des locations
//     Location loc1;
//     loc1.setFilePath("/");
//     loc1.setRoot("docs/fusion_web/", "docs/fusion_web/");
//     loc1.setIdxFiles({"index.html"});
//     server.setLocation("/", loc1);

//     Location loc2;
//     loc2.setFilePath("/tours");
//     loc2.setRoot("docs/fusion_web/tours", "docs/fusion_web/");
//     loc2.setIdxFiles({"tours1.html"});
//     server.setLocation("/tours", loc2);

//     // Affichage des valeurs pour vérification
//     std::cout << "Listen Port: " << server.getListenPort() << std::endl;
//     std::cout << "Host: " << server.getHost() << std::endl;
//     std::cout << "Server Name: " << server.getServerName() << std::endl;
//     std::cout << "Root: " << server.getRoot() << std::endl;

//     std::cout << "Error Pages: " << std::endl;
//     for (const auto &err_page : server.getErrPages())
//     {
//         std::cout << err_page.first << " -> " << err_page.second << std::endl;
//     }

//     std::cout << "Locations: " << std::endl;
//     for (const auto &location : server.getLocations())
//     {
//         std::cout << "File Path: " << location.getFilePath() << std::endl;
//         std::cout << "Root: " << location.getRoot() << std::endl;
//         std::cout << "Index Files: ";
//         for (const auto &file : location.getIdxFiles())
//         {
//             std::cout << file << " ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }
