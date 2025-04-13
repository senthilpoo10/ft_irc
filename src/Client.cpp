#include "../include/Client.hpp"

Client::Client() : _fd(-1), _authenticated(false) {}

Client::Client(int fd) : _fd(fd), _authenticated(false) {}

bool Client::isAuthenticated() const {
    return _authenticated;
}

void Client::authenticate() {
    _authenticated = true;
}

void Client::appendBuffer(const std::string& data) {
    _buffer += data;
}

bool Client::extractMessage(std::string& message) {
    size_t pos = _buffer.find("\r\n");
    if (pos == std::string::npos)
        return false;

    message = _buffer.substr(0, pos + 2);
    _buffer.erase(0, pos + 2);
    storeMessage(message);
    return true;
}

void Client::storeMessage(const std::string& message) {
    _messageQueue.push(message);
}

std::queue<std::string>& Client::getMessages() {
    return _messageQueue;
}


//Added for channel
void Client::setNickname(const std::string& nick) { _nickname = nick; }
std::string Client::getNickname() const { return _nickname; }

void Client::setUsername(const std::string& name) { _username = name; }
std::string Client::getUsername() const { return _username; }
