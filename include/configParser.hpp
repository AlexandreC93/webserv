#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sstream>

class LocationBlock {
public:
	LocationBlock();
	~LocationBlock();

	std::string location_path;
	std::string root;
	std::string index;
	std::string allow_methods;
	std::string autoindex;
	std::string upload_store;
	std::string cgi_path;
	std::string cgi_ext;
	std::string cgi;
	std::string ret;
};

class ServerBlock {
public:
	ServerBlock();
	~ServerBlock();

	int listen;
	std::string path;
	std::string server_name;
	std::string host;
	std::string root;
	std::string index;
	std::string default_server;
	std::string client_max_body_size;
	std::map<int, std::string> error_page;
	std::vector<LocationBlock> locations;
};

class ConfigParser {
public:
	ConfigParser(const std::string& filename);
	~ConfigParser();

	std::vector<ServerBlock> parseConfig();

private:
	std::ifstream file;
	std::vector<ServerBlock> serverBlocks;
	std::string filename;

	void parseServerBlock();
	void parseLocationBlock(ServerBlock& serverBlock);
	std::string getNextToken();
};

#endif
