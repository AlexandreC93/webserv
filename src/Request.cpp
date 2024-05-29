#include "../include/Request.hpp"
#include "../include/utils.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>

void Request::parse(const std::string &rawRequest)
{
    std::istringstream stream(rawRequest);
    std::string line;

    // Parse the request line
    std::getline(stream, line);
    std::istringstream requestLine(line);
    requestLine >> method >> uri;
    // std::cout << "line" << line << std::endl;
    // std::cout << "method" << method << std::endl;
    // std::cout << "uri" << uri << std::endl;

    // Parse the headers
    while (std::getline(stream, line) && line != "\r")
    {
        size_t pos = line.find(": ");
        if (pos != std::string::npos)
        {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 2);
            headers[headerName] = headerValue;
        }
    }

    // Parse the body
    if (headers.find("Content-Length") != headers.end())
    {
        std::string contentLength = headers["Content-Length"];

        int length = atoi(contentLength.c_str());
        body.resize(length);
        stream.read(&body[0], length);
        std::cout << "bodyparse" << body << "findubody" << std::endl;

        if (headers["Content-Type"].find("multipart/form-data") != std::string::npos)
        {
            parseMultipartFormData();
        }
    }
}

void Request::parseMultipartFormData()
{
    std::string boundary = "--" + headers["Content-Type"].substr(headers["Content-Type"].find("boundary=") + 9);
    std::istringstream stream(body);
    std::string line;
    std::string contentDisposition;
    std::string contentType;
    std::ostringstream valueStream;
    std::string filename;

    std::cout << "Boundary: " << boundary << std::endl;
    std::cout << "Body Content:\n"
              << body << std::endl; // Debug: print the entire body content

    while (std::getline(stream, line))
    {
        std::cout << "Line: " << line << std::endl; // Debug: print each line
        if (line.find(boundary) != std::string::npos)
        {
            std::getline(stream, contentDisposition);
            std::cout << "Content-Disposition: " << contentDisposition << std::endl; // Debug: print content disposition

            std::getline(stream, line); // Skip the empty line

            if (contentDisposition.find("filename=") != std::string::npos)
            {
                std::getline(stream, contentType);
                std::cout << "Content-Type: " << contentType << std::endl; // Debug: print content type

                std::getline(stream, line); // Skip the empty line
                valueStream.str("");        // Clear the valueStream for new content
                valueStream.clear();

                while (std::getline(stream, line) && line.find(boundary) == std::string::npos)
                {
                    valueStream << line << "\n";
                }

                std::string value = valueStream.str();
                if (!value.empty() && value[value.size() - 1] == '\n')
                {
                    value.erase(value.size() - 1);
                }

                size_t pos = contentDisposition.find("filename=");
                if (pos != std::string::npos)
                {
                    filename = contentDisposition.substr(pos + 10);
                    filename = filename.substr(0, filename.find("\""));
                    formData[filename] = value;

                    std::cout << "Filename: " << filename << std::endl; // Debug: print filename
                    std::cout << "File Content: " << value << std::endl;
                    std::cout << "File Content: " << value << std::endl; // Debug: print file content
                }
            }
        }
    }
}