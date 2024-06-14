#include "../include/Handler.hpp"
#include "../include/Request.hpp"
#include "../include/configParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

std::string handleGetRequest(ServerBlock &serverBlock, const std::string &uri)
{
	std::string filepath = "www/html" + uri; // ancien

	bool locationFound = false;
	// Parcourir tous les blocs de location pour trouver le chemin correspondant
	std::cout << "URI: " << uri << std::endl;
	for (std::vector<LocationBlock>::const_iterator it = serverBlock.locations.begin(); it != serverBlock.locations.end(); ++it)
	{
		std::cout << "Checking location: " << it->location_path << std::endl;
		std::cout << "Root for this location: " << it->root << std::endl;
		// if (uri.find(it->location_path) == 0) // Vérifie si l'URI commence par le chemin de la location
		if (it->location_path.find(uri) == 0)
		{
			// filepath = it->root + uri.substr(it->location_path.length());
			std::cout << "Filepath: " << filepath << std::endl;
			std::cout << "Matched location: " << it->location_path << ", filepath: " << filepath << std::endl;
			locationFound = true;
			break; // On a trouvé le chemin correspondant, pas besoin de continuer à chercher
		}
	}
	// Si aucun chemin de location n'a été trouvé, utiliser la racine du serveur par défaut
	if (!locationFound)
	{
		// filepath = serverBlock.root + uri;
		std::cout << "No location matched, using default root: " << serverBlock.root << ", filepath: " << filepath << std::endl;
	}
	if (filepath == "www/html/")
	{
		filepath += "index.html";
	}
	else if (filepath == "www/html" + uri)
	{
		// std::cout << "OK" << std::endl;
		filepath += ".html";
	}
	else
	{
		// filepath = "www/html/404.html"; // ancien
		int error_code = 404; // ou n'importe quel autre code d'erreur approprié
		if (serverBlock.error_page.find(error_code) != serverBlock.error_page.end())
		{
			filepath = serverBlock.error_page[error_code];
			// std::cout << "error: " << serverBlock.error_page[error_code] << std::endl; //test
		}
		else
		{
			filepath = "www/html/404.html"; // chemin par défaut si la page d'erreur n'est pas trouvée
		}
	}
	std::ifstream file(filepath.c_str());
	if (!file)
	{
		return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n404 - Not Found";
	}

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + to_string(content.length()) + "\r\n";
    response += "\r\n" + content;

    return response;
}

// std::string handleGetRequest(ServerBlock &serverBlock, const std::string &uri)
// {
// 	// std::string filepath = "www/html" + uri; //ancien

// 	std::string filepath;
// 	bool locationFound = false;
// 	// Parcourir tous les blocs de location pour trouver le chemin correspondant
// 	for (std::vector<LocationBlock>::const_iterator it = serverBlock.locations.begin(); it != serverBlock.locations.end(); ++it)
// 	{
// 		std::cout << "Checking location: " << it->location_path << std::endl;
// 		std::cout << "Root for this location: " << it->root << std::endl;
// 		if (uri.find(it->location_path) == 0) // Vérifie si l'URI commence par le chemin de la location
// 		{
// 			filepath = it->root + uri.substr(it->location_path.length());
// 			std::cout << "Matched location: " << it->location_path << ", filepath: " << filepath << std::endl;
// 			locationFound = true;
// 			break; // On a trouvé le chemin correspondant, pas besoin de continuer à chercher
// 		}
// 	}
// 	// Si aucun chemin de location n'a été trouvé, utiliser la racine du serveur par défaut
// 	if (!locationFound)
// 	{
// 		filepath = serverBlock.root + uri;
// 		std::cout << "No location matched, using default root: " << serverBlock.root << ", filepath: " << filepath << std::endl;
// 	}
// 	// Ajouter des fichiers spécifiques selon les chemins d'URI
// 	if (filepath == serverBlock.root + "/")
// 	{
// 		filepath += "index.html";
// 	}
// 	else if (filepath == serverBlock.root + "/test")
// 	{
// 		filepath += ".html";
// 	}
// 	else
// 	{
// 		// filepath = "www/html/404.html"; // ancien
// 		int error_code = 404; // ou n'importe quel autre code d'erreur approprié
// 		if (serverBlock.error_page.find(error_code) != serverBlock.error_page.end())
// 		{
// 			filepath = serverBlock.error_page[error_code];
// 			// std::cout << "error: " << serverBlock.error_page[error_code] << std::endl; //test
// 		}
// 		else
// 		{
// 			filepath = "www/html/404.html"; // chemin par défaut si la page d'erreur n'est pas trouvée
// 		}
// 	}
// 	std::ifstream file(filepath.c_str());
// 	if (!file)
// 	{
// 		return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n404 - Not Found";
// 	}

// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string content = buffer.str();

// 	std::string response = "HTTP/1.1 200 OK\r\n";
// 	response += "Content-Type: text/html\r\n";
// 	response += "Content-Length: " + to_string(content.length()) + "\r\n";
// 	response += "\r\n" + content;

// 	return response;
// }

std::string handlePostRequest(ServerBlock &serverBlock, const Request &request)
{
	// std::string uploadDir = "upload"; // ancien
	std::string uploadDir = "";
	// Parcourir les blocs de location pour trouver le répertoire de téléchargement
	for (std::vector<LocationBlock>::const_iterator it = serverBlock.locations.begin(); it != serverBlock.locations.end(); ++it)
	{
		if (!it->upload_store.empty())
		{
			uploadDir = it->upload_store;
			std::cout << "upload: " << it->upload_store << std::endl; // test
			break;
		}
	}
	// Vérifier si un répertoire de téléchargement a été trouvé
	if (uploadDir.empty())
	{
		return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 45\r\n\r\n<html><body>No upload_store defined</body></html>";
	}
	// Utiliser uploadDir pour enregistrer le fichier téléchargé
	std::string responseContent = saveUploadedFile(request, uploadDir);
	// Construire la réponse HTTP
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: " + std::to_string(responseContent.length()) + "\r\n";
	response += "\r\n" + responseContent;
	return response;
}

std::string handleDeleteRequest(const std::string &uri)
{
	std::string filepath = "." + uri;

	if (std::remove(filepath.c_str()) != 0)
	{
		std::cout << filepath.c_str() << std::endl;

		return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n404 - Not Found";
	}
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 0\r\n";
	response += "\r\n";

	return response;
}

std::string saveUploadedFile(const Request &request, const std::string &uploadDir)
{
	std::ostringstream responseStream;

	std::cout << "filename>" << std::endl;
	// std::cout << "reqform: " << request.formData << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.formData.begin(); it != request.formData.end(); ++it)
	{
		std::string filename = uploadDir + "/" + it->first;
		std::cout << "filename>" << filename << std::endl;
		std::cout << "itfirst>" << it->first << std::endl;
		std::ofstream outFile(filename.c_str(), std::ios::binary);

		if (!outFile)
		{
			responseStream << "Failed to upload file: " << it->first << "\n";
		}
		else
		{
			outFile.write(it->second.c_str(), it->second.size());
			outFile.close();
			responseStream << "File uploaded successfully: " << it->first << "\n";
		}
	}

	return responseStream.str();
}
