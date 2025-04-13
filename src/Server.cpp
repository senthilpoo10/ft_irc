#include "../include/Server.hpp"

Server::Server(int port, const std::string& password)
    : _port(port), _password(password) {
    initSocket();
}

Server::~Server() {
    close(_serverSocket);
    for (auto& p : _clients)
        close(p.first);
}

void Server::initSocket() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
        throw std::runtime_error("Ft_irc failed to create socket");

    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("Ft_irc failed to set socket options");

    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Ft_irc failed to bind socket");

    if (listen(_serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("Ft_irc failed to listen on socket");

    _pollFds.push_back({ _serverSocket, POLLIN, 0 });
    std::cout << "Ft_irc server started on port " << _port << std::endl;
}

void Server::run() {
    while (true) {
        int activity = poll(_pollFds.data(), _pollFds.size(), -1);
        if (activity < 0) {
            perror("poll error");
            continue;
        }

        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                if (_pollFds[i].fd == _serverSocket)
                    acceptNewClient();
                else
                    handleClientMessage(_pollFds[i].fd);
            }
        }
    }
}

void Server::acceptNewClient() {
    sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);
    int clientFd = accept(_serverSocket, (struct sockaddr*)&clientAddr, &addrlen);
    if (clientFd < 0) {
        perror("accept error");
        return;
    }

    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    _pollFds.push_back({ clientFd, POLLIN, 0 });
    _clients[clientFd] = Client(clientFd);
    std::cout << "New connection: fd=" << clientFd << std::endl;
}

void Server::disconnectClient(int clientFd) {
    std::cout << "Disconnecting fd=" << clientFd << std::endl;
    close(clientFd);
    _clients.erase(clientFd);
    _pollFds.erase(std::remove_if(_pollFds.begin(), _pollFds.end(), [clientFd](pollfd& pfd) {
        return pfd.fd == clientFd;
        }), _pollFds.end());
}

void Server::handleClientMessage(int clientFd) {
    char buffer[512];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        disconnectClient(clientFd);
        return;
    }

    buffer[bytesRead] = '\0';
    _clients[clientFd].appendBuffer(buffer);

    std::string message;
    // Clients get authenticated if they haven't been authenticated already
    //  or the message is passed for processing
    while (_clients[clientFd].extractMessage(message)) {
        if (!_clients[clientFd].isAuthenticated()) {
            if (message == "PASS " + _password + "\r\n") {
                _clients[clientFd].authenticate();
                std::cout << "Client " << clientFd << " authenticated." << std::endl;
            }
            else {
                std::cout << "Wrong password from client " << clientFd << std::endl;
                disconnectClient(clientFd);
                return;
            }
        }
        //The following branch is to integrate the server part & client processing
            // It retrieves the full message and prints it just for confirmation
        else {
            std::queue<std::string>& messages = _clients[clientFd].getMessages();
            while (!messages.empty()) {
                std::string msg = messages.front();
                messages.pop();
                std::cout << "Received message: " << msg << std::endl;
            }
        }
    }
}

