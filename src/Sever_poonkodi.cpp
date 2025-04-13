#include "../include/Headers.hpp"

bool shutDown;

Server::Server(std::string Port, std::string PassWord):_Port(Port), _PassWord(PassWord)
{
    shutDown = false;
    this->_serverName = "@HOP";

    creatSocket();
    if (fcntl(this->_server_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << RED << "FCNTL ERROR: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    struct pollfd server_pollfd;
    server_pollfd.fd = this->_server_fd;
    server_pollfd.events = POLLIN;
    this->_pollfds.push_back(server_pollfd); // add the server socket (fdSock) to pfds
    this->_fdNmbr = 1;   // Initially only the server socket is being monitored
    std::cout << YELLOW << "Server is setuped succefully" << RESET << std::endl;
    std::cout << YELLOW << "Server start listening in port " << Port << RESET << std::endl
              << std::endl;
}

void Server::creatSocket()
{
    int status;
    int yes = 1;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints)); // // Clear it
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // For binding server socket

    if ((status = getaddrinfo(NULL, this->_Port.c_str(), &hints, &res)) != 0)
    {
        std::cerr << RED << "getaddrinfo failed: " << gai_strerror(status) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    this->_server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // creates the server socket
    if (this->_server_fd < 0)
    {
        freeaddrinfo(res);
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        close(this->_server_fd);
        freeaddrinfo(res);
        std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl; 
        exit(EXIT_FAILURE);
    }
    if (bind(this->_server_fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(this->_server_fd);
        freeaddrinfo(res);
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(this->_server_fd, 10) == -1)
    {
        close(this->_server_fd);
        freeaddrinfo(res);
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);
}


void signalHandler(int i)
{
    (void)i;
    shutDown = true;
}

void shutDownserver()
{
    std::cout << std::endl;
    std::cout << RED << "Server shut down" << RESET << std::endl;
    exit(EXIT_SUCCESS);
}

void Server::addToPollfds(int newfd)
{
    struct pollfd fdNew;
    fdNew.fd = newfd;
    fdNew.events = POLLIN;
    this->_pollfds.push_back(fdNew);
    this->_fdNmbr++;
    this->_clientNBR++;
    this->clientList.insert(std::pair<int, Client >(newfd, Client(newfd)));
}

void Server::startServer()
{
    struct sockaddr_storage their_addr;
    int newfd;
    socklen_t addrlen;
    std::string msg;

    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, signalHandler);
    signal(SIGPIPE, SIG_IGN);

    while (!shutDown)
    {
        int poll_count = poll(&this->_pollfds.at(0), this->_fdNmbr, -1);
        if (poll_count == -1){ continue; }
        for (int i = 0; i < this->_fdNmbr; i++)
        {
            this->_index = 0;
            if (this->_pollfds.at(i).revents & POLLIN && !shutDown)
            {
                if (_pollfds[i].fd == this->_server_fd)
                {
                    addrlen = sizeof(their_addr);
                    newfd = accept(this->_server_fd, (struct sockaddr *)&their_addr, &addrlen);
                    if (newfd == -1)
                    {
                        perror("ACCEPT FAILS");
                    }
                    else
                    {
                        fcntl(newfd, F_SETFL, O_NONBLOCK);
                        addToPollfds(newfd);
                        clientMessage.insert(std::pair<int, std::string>(newfd, ""));
                        struct sockaddr_in *ip = (struct sockaddr_in *)&their_addr;
                        char clientIP[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(ip->sin_addr), clientIP, INET_ADDRSTRLEN);
                        this->clientList[newfd].setIp(clientIP);
                        std::cout << BLUE << "[" << this->getTime() << " Client " << newfd << "] " << RESET << "is connected to the server" << std::endl;
                        break;
                    }
                }
                else
                {
                    // other codes to write here
                }
            }
        }
    }
    if (shutDown)
    {
        shutDownserver();
    }
}

std::string Server::getTime()
{
    time_t ctime;
    struct tm *info;
    char    buff[100];

    time(&ctime);
    info = localtime(&ctime);

    strftime(buff, sizeof(buff), "%d-%m-%y %H:%M:%S",info);
    std::string tm(buff);
    return tm;
}

std::string Server::getHostName(){return this->_serverName;}
void    Server::setHostName(std::string name){this->_serverName = name;}

Server::~Server()
{
}