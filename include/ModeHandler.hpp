#pragma once

#include <string>
#include <map>
#include <vector>
#include "Channel.hpp"
#include "Client.hpp"

class ModeHandler 
{
	public:
		static std::string applyMode(Channel& ch, const std::string& flags, const std::vector<std::string>& params,
			std::map<int, Client>& clients, int senderFd);
};
