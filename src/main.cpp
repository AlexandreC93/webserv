#include "../include/Server.hpp"
#include "../include/configParser.hpp"
#include "../include/Handler.hpp"
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

// int main(int argc, char **argv) {
//     (void)argc;
//     (void)argv;

//     ConfigParser parser("./config/main.conf");
// 	std::vector<ServerBlock> serverBlocks = parser.parseConfig();
// 	parser.mainParse();
// 	// std::string configFile = (argc > 1) ? argv[1] : "conf/server.conf";
// 	// Server server(configFile);
// 	// server.start();
// 	// std::cout << "kugsdjkfgaskjgfghfashfhjs" << serverBlocks[0] << std::endl;

//     return 0;
// }

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    ConfigParser parser("./config/main.conf");
    parser.mainParse();  // Ensure this is called to parse the config
    std::vector<ServerBlock> serverBlocks = parser.parseConfig();

    for (std::vector<ServerBlock>::const_iterator it = serverBlocks.begin(); it != serverBlocks.end(); ++it) {
        // std::cout << "ICI" << std::endl;
		Server server(*it);
        server.start();
    }

    return 0;
}