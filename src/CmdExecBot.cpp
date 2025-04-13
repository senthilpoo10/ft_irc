#include "../include/CmdExec.hpp"

void CmdExec::helpBot(int fd)
{
	std::string msg;
	msg += "[BOT] IRC Command Help:\r\n";
	msg += " - JOIN #channel [key]      : Join a channel\r\n";
	msg += " - PART #channel            : Leave a channel\r\n";
	msg += " - PRIVMSG target :message  : Send a message\r\n";
	msg += " - TOPIC #channel [:topic]  : View/set channel topic\r\n";
	msg += " - MODE #channel +flags ... : Change channel modes\r\n";
	msg += " - INVITE nick #channel     : Invite user\r\n";
	msg += " - KICK #channel nick       : Kick user from channel\r\n";
	msg += " - NAMES #channel           : List users in channel\r\n";

	sendMsg(fd, msg);
}

