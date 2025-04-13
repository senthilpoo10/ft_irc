#pragma once

#include <string>
#include <map>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "../include/Utils.hpp"   // sendMsg
#include "../include/ModeHandler.hpp"


class CmdExec 
{
	private:

		std::map<int, Client>& clients;
		std::map<std::string, Channel>& channels;
		Server& server; 

		void join(int fd, std::istringstream& iss);
		void part(int fd, std::istringstream& iss);
		void message(int fd, std::istringstream& iss);
		void topic(int fd, std::istringstream& iss);
		void invite(int fd, std::istringstream& iss);
		void kick(int fd, std::istringstream& iss);
		void mode(int fd, std::istringstream& iss);
		void nick(int fd, std::istringstream& iss);
		void user(int fd, std::istringstream& iss);
		void listNames(int fd, std::istringstream& iss);
		void helpBot(int fd);

	public:

		CmdExec(std::map<int, Client>& c, std::map<std::string, Channel>& ch, Server& srv);
		void exec(int fd, const std::string& rawCmd);
};
