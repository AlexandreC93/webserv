#include "../include/Request.hpp"
#include "../include/utils.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void Request::parse(const std::string& rawRequest) {
    std::istringstream stream(rawRequest);
    std::string line;

    try {
        // Parse the request line
        std::getline(stream, line);
        std::istringstream requestLine(line);
        requestLine >> method >> uri;

        // Parse the headers
        while (std::getline(stream, line) && line != "\r") {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                std::string headerName = line.substr(0, pos);
                std::string headerValue = line.substr(pos + 2);
                headers[headerName] = headerValue;
            }
        }

        // Print headers for debugging
        std::cout << "Headers:" << std::endl;
        for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
            std::cout << it->first << "it: " << it->second << std::endl;
        }

        // Parse the body
        if (headers.find("Content-Length") != headers.end()) {
            std::string contentLength = headers["Content-Length"];
            int length = atoi(contentLength.c_str());
            body.resize(length);
            stream.read(&body[0], length);

            // Print body content for debugging
            std::cout << "Body Content Length: " << body.length() << std::endl;
            std::cout << "Body Content:\n" << body << std::endl;

            if (headers["Content-Type"].find("multipart/form-data") != std::string::npos) {
                parseMultipartFormData();
            }
        } else {
            std::cout << "No Content-Length header found." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing request: " << e.what() << std::endl;
    }
}

void Request::parseMultipartFormData() {
    try {
        std::string boundary = "--" + headers["Content-Type"].substr(headers["Content-Type"].find("boundary=") + 9);
        std::istringstream stream(body);
        std::string line;
        std::string contentDisposition;
        std::string contentType;
        std::ostringstream valueStream;
        std::string filename;

        std::cout << "Boundary: " << boundary << std::endl;

        while (std::getline(stream, line) && line != boundary + "--")
        {
            // trim(line); // Remove any leading/trailing whitespace
            std::cout << "LOOOOOOOOP" << std::endl;
            std::cout << "Line: " << line << " | Length: " << line.length() << std::endl;
            std::cout << "Boundary: " << boundary << " | Length: " << boundary.length() << std::endl;

            if (1)
            {
                std::getline(stream, contentDisposition);
                trim(contentDisposition); // Remove any leading/trailing whitespace
                std::getline(stream, contentType);
                trim(contentType); // Remove any leading/trailing whitespace

                std::getline(stream, line); // Skip the empty line
                std::ostringstream valueStream;
                while (std::getline(stream, line) && line != boundary && line != boundary + "--")
                {
                    valueStream << line << "\n";
                }
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

                    std::cout << "Filename: " << filename << std::endl;  // Debug: print filename
                    std::cout << "File Content: " << value << std::endl;  // Debug: print file content
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing multipart form data: " << e.what() << std::endl;
    }
}
