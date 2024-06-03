#include "../include/Server.hpp"
#include "../include/configParser.hpp"
#include "../include/Request.hpp"
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

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	(void)argv;
// 	// const std::string  name = "../config/default.conf";
// 	ConfigParser parser("../config/default.conf");
// 	// std::vector<ServerBlock> serverBlocks = parser.parseConfig();
// 	// signal(SIGINT, &handle_sigint);
// 	// signal(SIGPIPE, &handle_sigpipe);
// 	// std::string configFile = (argc > 1) ? argv[1] : "conf/server.conf";
// 	// Server server(configFile); // delete 
// 	//config OArse

// 	// server.start();
// 	return 0;
// }

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

	Server server("./config/main.conf");
    ConfigParser parser("./config/main.conf");
	std::vector<ServerBlock> serverBlocks = parser.parseConfig();
	parser.mainParse();
	server.start();
	std::cout << "kugsdjkfgaskjgfghfashfhjs" << std::endl;

    return 0;
}