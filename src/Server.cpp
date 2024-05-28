#include "../include/Server.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/Handler.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include <cstdio>

Server::Server(const std::string &configFile)
{
	loadConfig(configFile);
	// appelle le parsing du .conf
}

void Server::loadConfig(const std::string &configFile)
{
	// Implémentation pour charger le fichier de configuration
	(void)configFile; // Supprime l'avertissement de paramètre non utilisé pour l'instant
}

void Server::start()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port 8080" << std::endl;
	handleConnections();
}

void Server::handleConnections() {
    const int MAX_CLIENTS = 100;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (true) {
        int ret = poll(fds, nfds, -1);

        if (ret < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    int new_socket = accept(server_fd, NULL, NULL);
                    if (new_socket < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New connection accepted, fd: " << new_socket << std::endl;
                    fds[nfds].fd = new_socket;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } else {
                    char buffer[4096] = {0};
                    int valread = read(fds[i].fd, buffer, 4096);
                    if (valread < 0) {
                        perror("read");
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                    } else if (valread == 0) {
                        std::cout << "Connection closed, fd: " << fds[i].fd << std::endl;
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                    } else {
                        std::string rawRequest(buffer, valread);
                        Request request;
                        request.parse(rawRequest);

                        Response response;

                        if (request.method == "GET") {
                            std::string responseContent = handleGetRequest(request.uri);
                            response.status_code = 200;
                            response.body = responseContent;
                            response.headers["Content-Type"] = "text/html";
                            response.headers["Content-Length"] = to_string(responseContent.length());
                        } else if (request.method == "POST") {
                            std::string responseContent = handlePostRequest(request);
                            response.status_code = 200;
                            response.body = responseContent;
                            response.headers["Content-Type"] = "text/html";
                            response.headers["Content-Length"] = to_string(responseContent.length());
                        } else if (request.method == "DELETE") {
                            std::string responseContent = handleDeleteRequest(request.uri);
                            if (responseContent == "204 No Content") {
                                response.status_code = 204;
                                response.body = "";
                            } else {
                                response.status_code = 404;
                                response.body = responseContent;
                            }
                            response.headers["Content-Type"] = "text/html";
                            response.headers["Content-Length"] = to_string(response.body.length());
                        } else {
                            response.status_code = 405;
                            response.body = "405 - Method Not Allowed";
                            response.headers["Content-Type"] = "text/html";
                            response.headers["Content-Length"] = to_string(response.body.length());
                        }

                        std::string httpResponse = response.generate();
                        send(fds[i].fd, httpResponse.c_str(), httpResponse.length(), 0);
                        std::cout << "Response sent to fd: " << fds[i].fd << std::endl;
                    }
                }
            }
        }
    }
}
