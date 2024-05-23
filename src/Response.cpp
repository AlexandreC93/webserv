#include "Response.hpp"
#include <sstream>

std::string Response::generate() {
    std::ostringstream response;

    // Start line
    response << "HTTP/1.1 " << status_code << " " << get_status_message(status_code) << "\r\n";

    // Headers
    for (const auto& header : headers) {
        response << header.first << ": " << header.second << "\r\n";
    }

    // End of headers
    response << "\r\n";

    // Body
    response << body;

    return response.str();
}

std::string Response::get_status_message(int status_code) {
    switch (status_code) {
        case 200: return "OK";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        // Ajoute d'autres codes de statut selon les besoins
        default: return "Internal Server Error";
    }
}
