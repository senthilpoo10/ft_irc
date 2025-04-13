#pragma once

#include <string>
#include <queue>

class Client {
public:
    Client();
    Client(int fd);
    bool isAuthenticated() const;
    void authenticate();
    void appendBuffer(const std::string& data);
    bool extractMessage(std::string& message);
    void storeMessage(const std::string& message);
    std::queue<std::string>& getMessages();


	//Added for channel
	void setNickname(const std::string& nick);
    std::string getNickname() const;
    void setUsername(const std::string& name);
    std::string getUsername() const;


private:
    int _fd;
    bool _authenticated;
    std::string _buffer;
    std::queue<std::string> _messageQueue;

	//Added for channel
	std::string _nickname;
    std::string _username;

};
