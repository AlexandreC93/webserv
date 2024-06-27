#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <list>
#include <vector>
#include <netinet/in.h>
#include "configParser.hpp"
#include "LocationBlock.hpp"

class LocationBlock;

class ServerBlock
{
	public:
		ServerBlock();
		ServerBlock(ServerBlock const &src);
		~ServerBlock();

		ServerBlock &operator=(ServerBlock const &C);

		in_port_t getListen() const;
		int	getSocket() const;
		struct sockaddr_in getSockAddr() const;
		std::string getPath() const;
		std::string getServerName() const;
		in_addr_t getHost() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::string getDefaultServer() const;
		std::string getClientMaxBodySize() const;
		std::map<int, std::string> getErrorPage() const;
		std::vector<LocationBlock>& getLocations();
		const std::vector<LocationBlock>& getLocations() const;

		void setListen(in_port_t listen);
		void setSocket(int socket);
		void setSockaddr();
		void setPath(std::string path);
		void setServerName(std::string serverName);
		void setHost(in_addr_t host);
		void setRoot(std::string root);
		void setIndex(std::string index);
		void setDefaultServer(std::string defaultServer);
		void setClientMaxBodySize(std::string clientMaxBodySize);
		void setErrorPage(std::map<int, std::string> errorPage);
		void setLocations(std::vector<LocationBlock> location);
		void parseServerBlock(std::istringstream &cleanedStream);
		void bind_server();
		void handleConnections();
		void start();
		
		class ServerException : public std::exception {
			public :
				ServerException(std::string errMessage) throw() {
					_errMessage = "Server Error: " + errMessage;
				}
				virtual const char* what() const throw() {
					return (_errMessage.c_str());
				}
				~ServerException() throw() {}
			private:
				std::string _errMessage;
		};

	private:
		in_port_t _listen; //le port
		int _socket;
		int server_fd;
		struct sockaddr_in	_sockaddr;
		struct sockaddr_in address;
		std::string _path;
		std::string _server_name;
		in_addr_t _host;
		std::string _root;
		std::string _index;
		std::string _default_server;
		std::string _client_max_body_size;
		std::map<int, std::string> _error_page;
		std::vector<LocationBlock> _locations;
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
