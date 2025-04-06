#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
  private:
        std::string serverName;
        std::string Port;
        std::string PassWord;

  public:
        Server(std::string Port, std::string PassWord);
        ~Server();

        std::string getHostName();
        void    setHostName(std::string name);
        void    startServer();
};

#endif
