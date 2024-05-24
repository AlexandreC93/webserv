#include "../include/Server.hpp"

int main(int argc, char** argv) {
    std::string configFile = (argc > 1) ? argv[1] : "conf/server.conf";
    Server server(configFile);
    server.start();
    return 0;
}
