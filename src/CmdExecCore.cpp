#include "../include/CmdExec.hpp"

CmdExec::CmdExec(std::map<int, Client>& c, std::map<std::string, Channel>& ch, Server& srv)
	: clients(c), channels(ch), server(srv) {}

void CmdExec::exec(int fd, const std::string& rawCmd)
{
	try
	{
		std::istringstream iss(rawCmd);
		std::string cmd;
		iss >> cmd;
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

		if (!clients[fd].isAuthenticated())
		{
			try
			{
				sendMsg(fd, REPLY_ERR("Please authenticate first"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
			return;
		}

		if (cmd == "JOIN") join(fd, iss);
		else if (cmd == "PART") part(fd, iss);
		else if (cmd == "PRIVMSG") message(fd, iss);
		else if (cmd == "TOPIC") topic(fd, iss);
		else if (cmd == "INVITE") invite(fd, iss);
		else if (cmd == "KICK") kick(fd, iss);
		else if (cmd == "MODE") mode(fd, iss);
		else if (cmd == "NICK") nick(fd, iss);
		else if (cmd == "USER") user(fd, iss);
		else if (cmd == "NAMES") listNames(fd, iss);
		else if (cmd == "BOT") helpBot(fd);
		else
		{
			try
			{
				sendMsg(fd, REPLY_ERR("Unknown command"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "[ERROR] in exec(): " << e.what() << std::endl;
		try
		{
			sendMsg(fd, REPLY_ERR("Internal server error"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
	}
}

void CmdExec::nick(int fd, std::istringstream& iss)
{
	std::string nickname;
	iss >> nickname;

	if (nickname.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Missing nickname"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	for (auto& [otherFd, client] : clients)
	{
		if (client.getNickname() == nickname && otherFd != fd)
		{
			try
			{
				sendMsg(fd, REPLY_ERR("Nickname already in use"));
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
			}
			return;
		}
	}

	clients[fd].setNickname(nickname);
	try
	{
		sendMsg(fd, REPLY_OK("Nickname set successfully"));
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}

void CmdExec::user(int fd, std::istringstream& iss)
{
	std::string username;
	iss >> username;

	if (username.empty())
	{
		try
		{
			sendMsg(fd, REPLY_ERR("Missing username"));
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
		}
		return;
	}

	clients[fd].setUsername(username);
	clients[fd].authenticate();
	try
	{
		sendMsg(fd, REPLY_OK("User registered and authenticated"));
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "[send error] " << e.what() << " (fd: " << fd << ")\n";
	}
}

