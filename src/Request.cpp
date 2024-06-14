#include "../include/Request.hpp"
#include "../include/utils.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>

void Request::parse(const std::string &rawRequest)
{
	std::istringstream stream(rawRequest);
	std::string line;
	// while (std::getline(stream, line))
	// {
	// 	std::cout << line << std::endl;
	// }

	try
	{
		// Parse the request line
		std::getline(stream, line);
		std::istringstream requestLine(line);
		requestLine >> method >> uri;
		// std::cout << uri << std::endl;

		// Parse the headers
		while (std::getline(stream, line) && line != "\r")
		{
			size_t pos = line.find(": ");
			// std::cout << line << std::endl;
			if (pos != std::string::npos)
			{
				std::string headerName = line.substr(0, pos);
				std::string headerValue = line.substr(pos + 2);
				headers[headerName] = headerValue;
				// std::cout <<"KJDSFHSKDFHSJKD1" << headerName << std::endl;
				// std::cout <<"KJDSFHSKDFHSJKD2" << headerValue << std::endl;
			}
		}

		// Print headers for debugging
		std::cout << "Headers:" << std::endl;
		for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
		    std::cout << it->first << ": " << it->second << std::endl;
		}

		// Parse the body
		if (headers.find("Content-Length") != headers.end())
		{
			std::string contentLength = headers["Content-Length"];
			int length = std::atoi(contentLength.c_str());

			std::cout << "Reading body of length: " << length << std::endl;

			// Allocate the body buffer and read the content
			body.resize(length);
			stream.read(&body[0], length);

			// Print body content for debugging
			// std::cout << "Body Content Length: " << body.length() << std::endl;
			// std::cout << "Body Content:\n" << body << std::endl;

			if (headers["Content-Type"].find("multipart/form-data") != std::string::npos)
			{
				parseMultipartFormData();
			}
		}
		else
		{
			std::cout << "No Content-Length header found." << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error parsing request: " << e.what() << std::endl;
	}
}

void Request::parseMultipartFormData()
{
	std::string boundary = "--" + headers["Content-Type"].substr(headers["Content-Type"].find("boundary=") + 9);
	std::istringstream stream(body);
	std::string line;

	// while (std::getline(stream, line) && line != boundary) {
	//     // Skip the preamble
	// }
	// std::cout << "LOOOOOOOOP" << std::endl;

	while (std::getline(stream, line) /*&& line != boundary + "--"*/)
	{
		std::cout << "line: " << line << std::endl;
		std::cout << "boundary: " << boundary << std::endl;

		if (line == boundary)
		{
			std::string contentDisposition;
			std::getline(stream, contentDisposition);
			std::string contentType;
			std::getline(stream, contentType);

			std::getline(stream, line); // Skip the empty line
										// std::cout << "LINEbefore: " << line << std::endl;
			std::ostringstream valueStream;
			while (std::getline(stream, line) && line != boundary && line != boundary + "--")
			{
				valueStream << line << "\n";
			}
			std::cout << "LINEafter: " << line << std::endl;
			std::string value = valueStream.str();
			if (!value.empty() && value[value.size() - 1] == '\n')
			{
				value.erase(value.size() - 1);
			}

			// Extract filename from content disposition
			size_t pos = contentDisposition.find("filename=");
			if (pos != std::string::npos)
			{
				std::string filename = contentDisposition.substr(pos + 10);
				filename = filename.substr(0, filename.find("\""));

				formData[filename] = value;
			}
		}
	}
}
