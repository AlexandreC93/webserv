#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <string>
#include "./Server.hpp"
#include <list>
#include "./Client.hpp"
#include <vector>
#include <poll.h>
class Launcher // Gerer le parsing des servBlocks ainsi que l'init et le lancement du server (infinit loop)
{

private:
	std::vector<Server> _servers;
	std::list<Client> _clients;
	std::string _pathConf;
	std::vector<struct pollfd> _pollEvents;
	bool _breakLoop;

	Launcher(void);

public:
	// void parseLAUNCHER(const std::string& configBlock);
	Launcher(std::string path);
	Launcher(Launcher const &copy);
	~Launcher(void);
	void Launcher::parse(void);// confParser.parse
	void Launcher::lauchServers(void);
};

#endif
