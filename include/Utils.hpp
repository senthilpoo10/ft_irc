#pragma once

#include <string>
#include <map>
#include <sys/socket.h>
#include "Client.hpp"


#define REPLY_ERR(msg) std::string("Error: ") + msg + "\r\n"
#define REPLY_OK(msg) std::string("OK: ") + msg + "\r\n"
#define REPLY_INFO(msg) std::string(msg) + "\r\n"

int getFdByNick(const std::map<int, Client>& clients, const std::string& nick);
void sendMsg(int fd, const std::string& msg);

/*
** IRC Reply/Error Codes (Core set for ft_irc project)
**
** [Replies]
001 – RPL_WELCOME            : Welcome message after successful registration
331 – RPL_NOTOPIC            : Channel has no topic
332 – RPL_TOPIC              : Reply with topic of a channel
353 – RPL_NAMREPLY           : List of users in a channel
366 – RPL_ENDOFNAMES         : End of NAMES list

** [Errors]
401 – ERR_NOSUCHNICK         : No such nickname
403 – ERR_NOSUCHCHANNEL      : No such channel
404 – ERR_CANNOTSENDTOCHAN   : Cannot send to channel
431 – ERR_NONICKNAMEGIVEN    : No nickname provided
432 – ERR_ERRONEUSNICKNAME   : Invalid nickname
433 – ERR_NICKNAMEINUSE      : Nickname is already in use
441 – ERR_USERNOTINCHANNEL   : Target user is not in the channel
442 – ERR_NOTONCHANNEL       : You’re not in that channel
443 – ERR_USERONCHANNEL      : Target user is already in the channel
461 – ERR_NEEDMOREPARAMS     : Missing parameters
464 – ERR_PASSWDMISMATCH     : Password incorrect
482 – ERR_CHANOPRIVSNEEDED   : You’re not a channel operator
*/
