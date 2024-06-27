/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Launcher.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:51:19 by elie              #+#    #+#             */
/*   Updated: 2024/06/25 17:07:59 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCHER_HPP
# define LAUNCHER_HPP
#include "Server.hpp"
#include "configParser.hpp"
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <csignal>
# include <ctime>
# include <iomanip>
# include <cstdio>
# include <list>
# include <cmath>

# include <sys/types.h>
# include <sys/stat.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>
# include <sys/epoll.h>
# include <poll.h>
# include <sys/time.h>
# include <string.h>
# include <signal.h>
# include <dirent.h>
# include <fcntl.h>

class ServerBlock;
class LocationBlock;
class ConfigParser;

class Launcher
{
	private:
		std::vector<ServerBlock>	_servers;
		// std::list<Client>			_clients;
		std::string					_pathConf;
		std::vector<struct pollfd>	_pollEvent;
		bool						_breakPollLoop;
		Launcher(void);

	public:
		Launcher(std::string path);
		Launcher(Launcher const &C);
		~Launcher();

		Launcher &operator=(Launcher const &launcher);
		
		void parse();
		std::vector<ServerBlock> &getServers(void);
		std::vector<ServerBlock>::iterator getServerWithSameHostPort(std::vector<ServerBlock>::iterator it_find);
		// std::list<Client>				&getClients(void);
		void FolderTmpException() const;
		void initiateServersListening();
	
		class LauncherException : public std::exception {
		public :
			LauncherException(std::string errMessage) throw() {
				_errMessage = "Launcher Error: " + errMessage;
			}
			virtual const char* what() const throw() {
				return (_errMessage.c_str());
			}
			~LauncherException() throw() {}
		private:
			std::string _errMessage;
		};
		class SigException : public std::exception {};
};

#endif