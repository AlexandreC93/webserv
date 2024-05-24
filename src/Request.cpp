#include "../include/Request.hpp"
#include "../include/utils.hpp"
#include <sstream>

void Request::parse(const std::string& rawRequest) {
    std::istringstream stream(rawRequest);
    std::string line;

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

    // Parse the body
    if (headers.find("Content-Length") != headers.end()) {
        std::string contentLength = headers["Content-Length"];
        int length = string_to_int(contentLength);
        body.resize(length);
        stream.read(&body[0], length);
    }
}
