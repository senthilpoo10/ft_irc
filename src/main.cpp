#include "../include/Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port < 1024 || port > 65535) {
        std::cerr << "Invalid port number. Use a port between 1024 and 65535." << std::endl;
        return 1;
    }
    try {
        Server server(port, argv[2]);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
