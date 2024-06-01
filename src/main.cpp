#include "../include/Server.hpp"
#include <stdlib.h>
#include <signal.h>

void	handle_sigint(int sig)
{
	(void)sig;
}

void	handle_sigpipe(int sig)
{
	(void)sig;
}

int main(int argc, char **argv)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGPIPE, &handle_sigpipe);
	std::string configFile = (argc > 1) ? argv[1] : "conf/server.conf";
	Server server(configFile);
	server.start();
	return 0;
}