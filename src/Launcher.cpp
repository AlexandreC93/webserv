/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Launcher.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:51:17 by elie              #+#    #+#             */
/*   Updated: 2024/06/26 14:38:27 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Launcher.hpp"

Launcher::Launcher(std::string path)
{
	_pathConf = path;
	_breakPollLoop = false;
	_pollEvent.clear();
	return ;
}

Launcher::Launcher(Launcher const &C){
	*this = C;
}

Launcher::~Launcher(){	
}

Launcher & Launcher::operator=(Launcher const & launcher)
{
	if (this != &launcher)
	{
		_servers = launcher._servers;
		// _clients = launcher._clients;
		_pathConf = launcher._pathConf;
		_pollEvent = launcher._pollEvent;
		_breakPollLoop = launcher._breakPollLoop;
	}
	return (*this);
}

// GETTER //

std::vector<ServerBlock>	&Launcher::getServers(void) {

	return (this->_servers);
}

// std::list<Client>	&Launcher::getClients(void) {

// 	return (this->_clients);
// }

// ELSE //

void	Launcher::parse()
{
	ConfigParser config_parser(_pathConf);
	config_parser.parseConfig();
	_servers = config_parser.getServers();
	}

void Launcher::FolderTmpException(void) const
{
	std::string body_tmp_folder("tmp_body/");
	struct stat stats_dossier;

	if (stat(body_tmp_folder.c_str(), &stats_dossier) != 0
	|| access(body_tmp_folder.c_str(), W_OK | R_OK) != 0
	|| !S_ISDIR(stats_dossier.st_mode))
		throw LauncherException("Temp body folder does not exist");
}

std::vector<ServerBlock>::iterator Launcher::getServerWithSameHostPort(std::vector<ServerBlock>::iterator it_find)
{
	for (std::vector<ServerBlock>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		if (it == it_find)
			return (this->_servers.end());
		if (it->getHost() == it_find->getHost() && it->getListen())
			return (it);
	}
	return (this->_servers.end());
}

void Launcher::initiateServersListening()
{
	for (std::vector<ServerBlock>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		if (this->getServerWithSameHostPort(it) != this->_servers.end())
			it->setSocket(this->getServerWithSameHostPort(it)->getSocket());
		else
		{
			int listensocket = socket(AF_INET, SOCK_STREAM, 0);
			if (listensocket < 0)
				throw LauncherException("Problem with socket !");
			const int enable = 1;
			if (setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				throw LauncherException("Problem with setsockopt !");
			it->setSocket(listensocket);
			it->setSockaddr();
			it->bind_server();
			if (listen(it->getSocket(), 50) < 0) //50 = MAX_WAIT
				throw LauncherException("Impossible to listen !");
		}
		// addFdToPollIn(it->getListenSocket());
		char	str_ip_server[INET_ADDRSTRLEN];
		struct sockaddr_in  server_addr = it->getSockAddr();
		inet_ntop (AF_INET, &(server_addr.sin_addr), str_ip_server, sizeof (str_ip_server));
	}
}