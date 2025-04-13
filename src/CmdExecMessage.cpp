#include "../include/CmdExec.hpp"

void CmdExec::message(int fd, std::istringstream& iss)
{
	std::string target;
	iss >> target;
	std::string msg;
	getline(iss, msg);

	if (target.empty() || msg.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("PRIVMSG requires target and message"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	if (target[0] == '#')
	{
		if (channels.find(target) == channels.end())
		{
			try
			{
				sendMsg(fd, REPLY_ERR("Channel not found"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
			return;
		}

		Channel& ch = channels[target];
		if (!ch.isMember(fd))
		{
			try
			{
				sendMsg(fd, REPLY_ERR("You are not in the channel"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
			return;
		}

		std::string fullMsg = ":" + clients[fd].getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
	}
	else
	{
		int targetFd = getFdByNick(clients, target);
		if (targetFd == -1)
		{
			try
			{
				sendMsg(fd, REPLY_ERR("User not found"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
			return;
		}

		std::string fullMsg = ":" + clients[fd].getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
		try
		{
			sendMsg(targetFd, fullMsg);
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[DM send failed] " << e.what() << " (fd: " << targetFd << ")\n";
		}
	}
}

void CmdExec::topic(int fd, std::istringstream& iss)
{
	std::string chName;
	iss >> chName;
	std::string newTopic;
	getline(iss, newTopic);

	if (chName.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Missing channel name"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (channels.find(chName) == channels.end())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Channel not found"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	Channel& ch = channels[chName];
	if (!ch.isMember(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("You are not in the channel"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	std::string nick = clients[fd].getNickname();

	if (newTopic.empty())
	{
		try
		{
			if (ch.getTopic().empty())
			{
				sendMsg(fd, ":ft_irc 331 " + nick + " " + chName + " :No topic is set\r\n");
			} //just ex server
			else
			{
				sendMsg(fd, ":ft_irc 332 " + nick + " " + chName + " :" + ch.getTopic() + "\r\n");
			}//just ex server
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
	}
	else if (ch.isOp(fd))
	{
		newTopic.erase(0, newTopic.find(':') + 1);
		ch.setTopic(newTopic);

		std::string topicBroadcast = ":" + nick + " TOPIC " + chName + " :" + newTopic + "\r\n";

		try
		{
			sendMsg(fd, topicBroadcast);
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
	}
	else
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Only channel operator can set topic"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
	}
}

void CmdExec::listNames(int fd, std::istringstream& iss)
{
	std::string chName;
	iss >> chName;

	if (chName.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Missing channel name"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (channels.find(chName) == channels.end())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Channel not found"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	Channel& ch = channels[chName];
	std::string userList;

	for (int userFd : ch.getMembers())
	{
		std::string nick = clients[userFd].getNickname();
		if (!nick.empty())
		{
			if (ch.isOp(userFd)) userList += "@";
			userList += nick + " ";
		}
	}

	std::string senderNick = clients[fd].getNickname();

	try
	{
		sendMsg(fd, ":ft_irc 353 " + senderNick + " = " + chName + " :" + userList + "\r\n"); //just ex server
		sendMsg(fd, ":ft_irc 366 " + senderNick + " " + chName + " :End of /NAMES list\r\n"); //just ex server
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}
