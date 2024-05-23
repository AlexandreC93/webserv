#include "../include/Handler.hpp"
#include <fstream>
#include <sstream>

std::string handleGetRequest() {
    std::ifstream file("www/html/index.html");
    if (!file) {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n404 - Not Found";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(content.length()) + "\r\n";
    response += "\r\n" + content;

    return response;
}
