#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <netinet/in.h>
#include "Location.hpp"

class Server
{
	public:
		Server(const std::string& configFile);
		void start();

	private:
		int server_fd;
		struct sockaddr_in address;
		// std::map<std::string, LocationConfig> locations;

		void loadConfig(const std::string& configFile);
		void handleConnections();
		void handleRequest(int client_socket);
};

#endif
