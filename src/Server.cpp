#include "Server.hpp"
#include "Handler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Server::Server(const std::string& configFile) {
    loadConfig(configFile);
}

void Server::loadConfig(const std::string& configFile) {
    // Implémentation pour charger le fichier de configuration
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
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }
        handleRequest(new_socket);
        close(new_socket);
    }
}

void Server::handleRequest(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    std::cout << "Request received:\n" << buffer << std::endl;

    std::string response = handleGetRequest();
    send(client_socket, response.c_str(), response.length(), 0);
}
