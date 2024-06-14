#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <netinet/in.h>
#include "Location.hpp"
#include "configParser.hpp"

class Server // On instancie la class Server for each server blocks
{
public:
	// Server(const ServerBlock &serverBlock);
	// void start();

private:
	in_port_t _port;
	in_addr_t _host;
	struct sockaddr_in _servaddr;
	std::string _serverName;
	std::string _root;
	unsigned int _clientMaxBodySize;
	std::string _index;
	std::map<int, std::string> _errorPages;
	std::vector<Location> _locations;
	int _listenSocket;
	static bool _verbose;
	int _timeout;
};

#endif
