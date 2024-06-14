#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Client.hpp"
#include "./Server.hpp"
#include <string>
#include <list>
#include <vector>

class Client // incomplet 
{

private: //Launcher contient une list de clients 
	// enum for clients status
	// HttpReq
	// HttpRes
	std::string _incoming_msg;
	std::vector<Server *> _server_ptr;
	// struct sockadrr_in _client_adrr;
	std::vector<char> _req_receive;
	std::string _req_header;
	Launcher *_launcher;

public:
	// void parseLAUNCHER(const std::string& configBlock);
};

#endif
