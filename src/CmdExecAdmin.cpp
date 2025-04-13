#include "../include/CmdExec.hpp"

void CmdExec::invite(int fd, std::istringstream& iss)
{
	std::string nick, chName;
	iss >> nick >> chName;

	if (nick.empty() || chName.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("INVITE requires nickname and channel"));
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
	if (!ch.isOp(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Only operator can invite"));
		} catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	int targetFd = getFdByNick(clients, nick);
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

	ch.invite(targetFd);
	try
	{
		sendMsg(targetFd, REPLY_INFO("You are invited to " + chName));
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << targetFd << ")\n";
	}
}

void CmdExec::kick(int fd, std::istringstream& iss)
{
	std::string chName, targetNick;
	iss >> chName >> targetNick;

	if (chName.empty() || targetNick.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("KICK requires channel and nickname"));
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
	if (!ch.isOp(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Only channel operator can kick"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	int targetFd = getFdByNick(clients, targetNick);
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

	if (!ch.isMember(targetFd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("User is not in the channel"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	ch.removeMember(targetFd);
	std::string msg = ":" + clients[fd].getNickname() + " KICK " + chName + " " + targetNick + " :Kicked\r\n";

	try
	{
		sendMsg(targetFd, msg);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << targetFd << ")\n";
	}
}

void CmdExec::mode(int fd, std::istringstream& iss)
{
	std::string chName, modeFlags;
	iss >> chName >> modeFlags;

	std::vector<std::string> params;
	std::string p;
	while (iss >> p) params.push_back(p);

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
		} catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (!ch.isOp(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Only operator can change mode"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	std::string result = ":" + clients[fd].getNickname() + " " +
		ModeHandler::applyMode(ch, modeFlags, params, clients, fd);

	try
	{
		sendMsg(fd, result);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}
