#include "../include/ModeHandler.hpp"

std::string ModeHandler::applyMode(Channel& ch, const std::string& flags, const std::vector<std::string>& params,
	std::map<int, Client>& clients, int senderFd)
{
	bool add = true;
	std::string flagStr;
	std::vector<std::string> usedParams;
	size_t idx = 0;

	for (size_t i = 0; i < flags.size(); ++i) {
		char f = flags[i];
		if (f == '+') { add = true; flagStr += '+'; continue; }
		if (f == '-') { add = false; flagStr += '-'; continue; }

		flagStr += f;

		switch (f) {
			case 'i': ch.setInviteOnly(add); break;
			case 't': ch.lockTopic(add); break;
			case 'k':
				if (add) {
					if (idx >= params.size()) break;
					ch.setPassword(params[idx]); ch.enablePassword(true);
					usedParams.push_back(params[idx++]);
				} else {
					ch.setPassword(""); ch.enablePassword(false);
				}
				break;
			case 'l':
				if (add) {
					if (idx >= params.size()) break;
					ch.setLimit(std::atoi(params[idx].c_str())); ch.enableLimit(true);
					usedParams.push_back(params[idx++]);
				} else {
					ch.setLimit(-1); ch.enableLimit(false);
				}
				break;
			case 'o':
				if (idx >= params.size()) break;
				for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
					if (it->second.getNickname() == params[idx]) {
						if (add) ch.addOp(it->first);
						else ch.removeOp(it->first);
						usedParams.push_back(params[idx]);
						break;
					}
				}
				++idx;
				break;
		}
	}

	std::string resp = "MODE " + ch.getName() + " " + flagStr;
	for (size_t i = 0; i < usedParams.size(); ++i)
		resp += " " + usedParams[i];

	return resp + "\r\n";
}
