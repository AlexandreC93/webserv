#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sstream>
#include "Server.hpp"

class ConfigParser // parse le .conf
{
private:
	ConfigParser();
	std::string _path;
	std::vector<Server> _servers;
	std::vector<std::string> _serverConf;
	int _serverNb;

public:
	ConfigParser(std::string path);
	ConfigParser(ConfigParser const &src);
	~ConfigParser();

	ConfigParser &operator=(ConfigParser const &rhs);

	std::string getPath() const;
	std::vector<Server> getServers() const;
	std::vector<std::string> getServerConf() const;
	int getServerNb() const;

	void parse(void);
	void splitServerBlocks(std::string &content);
	//configure servers
	class ConfParserException : public std::exception
	{
	public:
		ConfParserException(std::string errMessage) throw()
		{
			_errMessage = "Parse Error: " + errMessage;
		}
		virtual const char *what() const throw()
		{
			return (_errMessage.c_str());
		}
		~ConfParserException() throw() {}

	private:
		std::string _errMessage;
	};
};

#endif








// class LocationBlock {
// public:
// 	LocationBlock();
// 	~LocationBlock();

// 	std::string location_path;
// 	std::string root;
// 	std::string index;
// 	std::string allow_methods;
// 	std::string autoindex;
// 	std::string upload_store;
// 	std::string cgi_path;
// 	std::string cgi_ext;
// 	std::string cgi;
// 	std::string ret;
// };

// class ServerBlock {
// public:
// 	ServerBlock();
// 	~ServerBlock();

// 	int listen;
// 	std::string path;
// 	std::string server_name;
// 	std::string host;
// 	std::string root;
// 	std::string index;
// 	std::string default_server;
// 	std::string client_max_body_size;
// 	std::map<int, std::string> error_page;
// 	std::vector<LocationBlock> locations;
// };

// class ConfigParser {
// public:
// 	ConfigParser(const std::string& filename);
// 	~ConfigParser();
// 	void mainParse(void);
// 	// std::vector<ServerBlock> getServerBlocks() const;
// 	std::vector<ServerBlock> parseConfig();

// private:
// 	std::ifstream file;
// 	std::string filename;
// 	std::vector<ServerBlock> serverBlocks;

// 	void parseServerBlock();
// 	void parseLocationBlock(ServerBlock& serverBlock);
// 	std::string getNextToken();
// };