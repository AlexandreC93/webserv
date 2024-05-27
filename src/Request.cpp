#include "../include/Request.hpp"
#include "../include/utils.hpp"
#include <sstream>
#include <iostream>

void Request::parse(const std::string &rawRequest)
{
	std::istringstream stream(rawRequest);
	std::string line;

	// Parse the request line
	std::getline(stream, line);
	std::istringstream requestLine(line);
	requestLine >> method >> uri;

	// Parse the headers
	while (std::getline(stream, line) && line != "\r")
	{
		std::cout << "line>> " << line << std::endl;
		size_t pos = line.find(": ");
		if (pos != std::string::npos)
		{
			std::string headerName = line.substr(0, pos);
			std::string headerValue = line.substr(pos + 2);
			headers[headerName] = headerValue;
			// std::cout << "headerName>> " << headerName << std::endl;
			// std::cout << "headerValue>> " << headerValue << std::endl;
		}
	}

	// Parse the body
	if (headers.find("Content-Length") != headers.end())
	{
		std::string contentLength = headers["Content-Length"];
			std::cout << "contentLength>> " << contentLength << std::endl;
		int length = string_to_int(contentLength);
		body.resize(length);
		stream.read(&body[0], length);
	}
}
