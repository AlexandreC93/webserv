#include "../include/Server.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/Handler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>  // Pour EXIT_FAILURE et exit

Server::Server(const std::string& configFile) {
    loadConfig(configFile);
}

void Server::loadConfig(const std::string& configFile) {
    // Implémentation pour charger le fichier de configuration
    (void)configFile;  // Supprime l'avertissement de paramètre non utilisé pour l'instant
}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port 8080" << std::endl;
    handleConnections();
}

void Server::handleConnections() {
    while (true) {
        int new_socket = accept(server_fd, NULL, NULL);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }
        handleRequest(new_socket);
        close(new_socket);
    }
}

void Server::handleRequest(int client_socket) {
    char buffer[4096] = {0};
    int valread = read(client_socket, buffer, 4096);
    if (valread < 0) {
        perror("read");
        return;
    }

    std::string rawRequest(buffer);
    Request request;
    request.parse(rawRequest);

    Response response;

    if (request.method == "GET") {
        std::string responseContent = handleGetRequest(request.uri);
        response.status_code = 200;
        response.body = responseContent;
        response.headers["Content-Type"] = "text/html";
        response.headers["Content-Length"] = to_string(responseContent.length());
    } else {
        response.status_code = 405;
        response.body = "405 - Method Not Allowed";
        response.headers["Content-Type"] = "text/html";
        response.headers["Content-Length"] = to_string(response.body.length());
    }

    std::string httpResponse = response.generate();
    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}
