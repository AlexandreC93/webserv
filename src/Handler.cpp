#include "../include/Handler.hpp"
#include "../include/Request.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

std::string handleGetRequest(const std::string &uri)
{
	std::string filepath = "www/html" + uri;
	std::cout << "uri>>" << filepath << std::endl;
	if (filepath == "www/html/")
	{
		filepath += "index.html";
	}
	else if (filepath == "www/html/test")
	{
		filepath += ".html";
	}
	else
	{
		filepath = "www/html/404.html";
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

std::string handlePostRequest(const Request &request)
{
	std::string uploadDir = "uploads";
    std::string responseContent = saveUploadedFile(request, uploadDir);
	// std::cout << request.body << std::endl;
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: " + to_string(responseContent.length()) + "\r\n";
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

std::string saveUploadedFile(const Request& request, const std::string& uploadDir) {
    std::ostringstream responseStream;

		// std::cout << "filename>" << std::endl;

    for (std::map<std::string, std::string>::const_iterator it = request.formData.begin(); it != request.formData.end(); ++it) {
        std::string filename = uploadDir + "/" + it->first;
		std::cout << "filename>" << filename << std::endl;
		std::cout << "itfirst>" << it->first << std::endl;
        std::ofstream outFile(filename.c_str(), std::ios::binary);

        if (!outFile) {
            responseStream << "Failed to upload file: " << it->first << "\n";
        } else {
            outFile.write(it->second.c_str(), it->second.size());
            outFile.close();
            responseStream << "File uploaded successfully: " << it->first << "\n";
        }
    }

    return responseStream.str();
}