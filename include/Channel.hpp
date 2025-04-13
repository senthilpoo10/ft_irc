#pragma once

#include <string>
#include <set>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

class Channel 
{
	private:

		std::string name;
		std::string topic;
		std::string password;

		int limit;

		bool inviteOnly;
		bool topicLock;
		bool hasPassword;
		bool limitEnabled;

		std::set<int> members;
		std::set<int> ops;
		std::set<int> invited;

	public:

		Channel();
		Channel(const std::string& name);

		const std::string& getName() const;
		const std::string& getTopic() const;
		void setTopic(const std::string& topic);

		void addMember(int fd);
		void removeMember(int fd);
		bool isMember(int fd) const;

		void addOp(int fd);
		void removeOp(int fd);
		bool isOp(int fd) const;

		void setPassword(const std::string& pw);
		const std::string& getPassword() const;
		bool checkPassword(const std::string& pw) const;

		void setInviteOnly(bool mode);
		bool isInviteOnly() const;

		void setLimit(int limit);
		bool isFull() const;

		void invite(int fd);
		bool isInvited(int fd) const;

		void broadcast(const std::string& msg, int senderFd) const;
		void sendJoinNotice(int senderFd, const std::string& nick);

		const std::set<int>& getMembers() const;

		void lockTopic(bool mode);
		bool isTopicLocked() const;

		void enablePassword(bool mode);
		bool isPasswordEnabled() const;

		void enableLimit(bool mode);
		bool isLimitEnabled() const;

		std::string getModeString() const;

};
