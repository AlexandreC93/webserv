
#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

#include <string>
#include "configLocation.hpp"
#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include "configLocation.hpp"

class Server
{
	public:
		Server(const std::string& configFile);
		Server(Server const &C);
		~Server();

		Server &operator=(Server const &C);

		in_port_t getListenPort() const;
		in_addr_t getHost() const;
		std::string getServerName() const;
		std::string getRoot() const;
		std::map<int, std::string> getErrPages() const;
		std::vector<Location> getLocations() const;

		void setListenPort(std::string listen_port);
		void setHost(std::string host);
		void setServerName(std::string server_name);
		void setRoot(std::string root);
		void setErrPages(std::vector<std::string> err_pages);
		void setLocation(std::string path, Location location);

	private:
		in_port_t listen_port;
		in_addr_t host;
		std::string server_name;
		std::string root;
		std::map<int, std::string> err_pages;
		std::vector<Location> locations;
};

#endif
