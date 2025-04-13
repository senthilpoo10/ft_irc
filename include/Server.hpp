#pragma once

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <algorithm>

class Server {
public:
    Server(int port, const std::string& password);
    ~Server();
    void run();

private:
    int _serverSocket;
    int _port;
    std::string _password;
    std::map<int, Client> _clients;
    std::vector<struct pollfd> _pollFds;

    void initSocket();
    void acceptNewClient();
    void disconnectClient(int clientFd);
    void handleClientMessage(int clientFd);
};
