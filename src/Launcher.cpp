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
	for (std::list<Client>::iterator it = this->_clients.begin();
		it != this->_clients.end(); ++it)
		close(it->getComSocket());
	for (std::vector<Server>::iterator it = this->_servers.begin();
		it != this->_servers.end(); ++it)
		close(it->getListenSocket());
}

Launcher &Launcher::operator=(Launcher const &launcher)
{
	if (this != &launcher)
	{
		_servers = launcher._servers;
		_clients = launcher._clients;
		_pathConf = launcher._pathConf;
		_pollEvents = launcher._pollEvents;
		_breakLoop = launcher._breakLoop;
	}
	return (*this);
}
