#ifndef SERVER_HPP
#define SERVER_HPP

class Client;

class Server
{
    private:
        std::string _serverName;
        int _server_fd; // server socket file descriptor
        std::vector<struct pollfd> _pollfds; // list of all file descriptors (FDs)
        int _fdNmbr; // how many file descriptors (clients + server socket) are active.
        std::string _Port;
        std::string _PassWord;
        int _clientNBR;
        std::map<int , Client> clientList;
        std::map<int, std::string> clientMessage;
        int _index;
             
        void    creatSocket();
        std::string getTime();
        void    addToPollfds(int newfd);

    public:
        Server(std::string Port, std::string PassWord);
        ~Server();

        std::string getHostName();
        void    setHostName(std::string name);
        void    startServer();

};

#endif
