#include "../include/Utils.hpp"

int getFdByNick(const std::map<int, Client>& clients, const std::string& nick)
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNickname() == nick)
			return it->first;
	}
	return -1;
}

#include <stdexcept>  // for std::runtime_error

void sendMsg(int fd, const std::string& msg)
{
	ssize_t sent = send(fd, msg.c_str(), msg.length(), 0);
	if (sent < 0)
	{
// #ifdef DEBUG_SEND
// 		std::cerr << "[sendMsg error] fd=" << fd << " msg=" << msg << std::endl;
// #endif
		throw std::runtime_error("sendMsg failed: client unreachable or disconnected");
	}
// #ifdef DEBUG_SEND
// 	std::cout << "[→ fd " << fd << "] " << msg << std::endl;
// #endif
} // CXXFLAGS += -Wall -Wextra -Werror -DDEBUG_SEND






