#include "../include/CmdExec.hpp"

void CmdExec::join(int fd, std::istringstream& iss)
{
	std::string chName, key;
	iss >> chName >> key;

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
	if (chName[0] != '#')
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Invalid channel format (use #name)"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	Channel& ch = channels[chName];

	if (ch.getName().empty())
	{
		ch = Channel(chName);
		ch.addOp(fd);
	}

	if (ch.isInviteOnly() && !ch.isInvited(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Channel is invite-only"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (!ch.getPassword().empty() && !ch.checkPassword(key))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Incorrect channel password"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (ch.isFull())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Channel is full"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}
	if (ch.isMember(fd))
	{
		try
		{
			sendMsg(fd, REPLY_ERR("You are already in the channel"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	ch.addMember(fd);

	try
	{
		sendMsg(fd, REPLY_OK("You joined " + chName));
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}

void CmdExec::part(int fd, std::istringstream& iss)
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
			sendMsg(fd, REPLY_ERR("Channel does not exist"));
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
			sendMsg(fd, REPLY_ERR("You are not in this channel"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	ch.removeMember(fd);
	try
	{
		sendMsg(fd, REPLY_OK("You left " + chName));
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}
