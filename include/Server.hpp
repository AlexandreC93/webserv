#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <netinet/in.h>
#include "LocationConfig.hpp"
#include "configParser.hpp"

class Server {
public:
    Server(const ServerBlock& serverBlock);
    void start();

private:
    int server_fd;
    struct sockaddr_in address;
    ServerBlock serverBlock;  // Store the ServerBlock configuration

    void handleConnections();
};

// class Server {
// public:
//     Server(const std::string& configFile);
//     void start();

// private:
//     int server_fd;
//     struct sockaddr_in address;
//     std::map<std::string, LocationConfig> locations;
//     std::map<int, std::string> error_pages;

//     // void loadConfig(const std::string& configFile);
//     void handleConnections();
//     // void handleRequest(int client_socket);
// };

#endif
