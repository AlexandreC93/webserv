#include "../include/Server.hpp"
#include "../include/configParser.hpp"
#include "../include/Handler.hpp"
#include <stdlib.h>
#include <signal.h>

// void	handle_sigint(int sig)
// {
// 	(void)sig;
// }

// void	handle_sigpipe(int sig)
// {
// 	(void)sig;
// }

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    ConfigParser parser("./config/main.conf");
    std::vector<ServerBlock> serverBlocks = parser.parseConfig();
    parser.mainParse();  // Ensure this is called to parse the config

    for (std::vector<ServerBlock>::const_iterator it = serverBlocks.begin(); it != serverBlocks.end(); ++it) {
        // std::cout << "ICI" << std::endl;
		ServerBlock server(*it);
        server.start();
    }

    return 0;
}
// int main(int argc, char **argv) {

//     if (argc > 2){
//         std::cerr << "Error: Too many arguments" << std::endl;
//         return (1);
//     }
//     std::string path;
//     if (argc == 1)
//         path = "./config/default.conf";
//     else
//         path = argv[1];
//     try{
//         signal(SIGINT, &handle_sigint);
//         signal(SIGPIPE, &handle_sigpipe);
//         // Launcher.test(.conf)
//         // Launcher.parser();
//         // Launcher.servers();
//     }
//     catch (SigException &e) {//A AJOUTER DANS UNE CLASSE
//         std::cerr << std::endl << "Ciao!" << std::endl;
//         return (EXIT_SUCCESS);
//     }
//     catch (CGI::CGIexception &e){ // A FAIRE
//         return (EXIT_SUCCESS);
//     }
//     catch (std::exception &e){
//         std::cerr << e.what() << std::endl;
//         return (EXIT_FAILURE); // A REVOIR
//     }
//     return 0;
// }