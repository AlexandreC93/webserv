#include "../include/Handler.hpp"
#include <fstream>
#include <sstream>
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
