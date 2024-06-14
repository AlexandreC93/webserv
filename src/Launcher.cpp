#include "../include/Launcher.hpp"

Launcher::Launcher(std::string path)
{
	this->_pathConf = path;
	_breakLoop = false;
	_pollEvents.clear();
	return ;
}

Launcher::Launcher(Launcher const & copy)
{
	*this = copy;
}

Launcher::~Launcher(void)
{
	// for (std::list<Client>::iterator it = this->_clients.begin();
	// 	it != this->_clients.end(); ++it)
	// 	close(it->getComSocket());
	// for (std::vector<Server>::iterator it = this->_servers.begin();
	// 	it != this->_servers.end(); ++it)
	// 	close(it->getListenSocket());
}
