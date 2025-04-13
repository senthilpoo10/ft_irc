#include "../include/Channel.hpp"
#include "../include/Utils.hpp"
#include <set> 

Channel::Channel()
	: name(""), limit(-1), inviteOnly(false),
	  topicLock(false), hasPassword(false), limitEnabled(false) {}

Channel::Channel(const std::string& name)
	: name(name), limit(-1), inviteOnly(false),
	  topicLock(false), hasPassword(false), limitEnabled(false) {}

const std::string& Channel::getName() const { return name; }

const std::string& Channel::getTopic() const { return topic; }

void Channel::setTopic(const std::string& t) { topic = t; }

void Channel::addMember(int fd) { members.insert(fd); }

void Channel::removeMember(int fd)
{
	members.erase(fd);
	ops.erase(fd);
	invited.erase(fd);
}

bool Channel::isMember(int fd) const { return members.count(fd); }

void Channel::addOp(int fd) { ops.insert(fd); }

void Channel::removeOp(int fd) { ops.erase(fd); }

bool Channel::isOp(int fd) const { return ops.count(fd); }

void Channel::setPassword(const std::string& pw)
{
	password = pw;
	hasPassword = !pw.empty();
}

const std::string& Channel::getPassword() const { return password; }

bool Channel::checkPassword(const std::string& pw) const { return password == pw; }

void Channel::setInviteOnly(bool mode) { inviteOnly = mode; }

bool Channel::isInviteOnly() const { return inviteOnly; }

void Channel::setLimit(int l)
{
	limit = l;
	limitEnabled = (l > 0);
}

bool Channel::isFull() const
{
	return (limit != -1) && ((int)members.size() >= limit);
}

void Channel::invite(int fd) { invited.insert(fd); }

bool Channel::isInvited(int fd) const { return invited.count(fd); }

void Channel::broadcast(const std::string& msg, int senderFd) const 
{
	for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		if (*it != senderFd)
		{
			try
			{
				sendMsg(*it, msg);
			}
			catch
			(const std::runtime_error& e)
			{
				std::cerr << "[broadcast send error] " << e.what() << " (fd: " << *it << ")\n";
			}
		}
	}
}

void Channel::sendJoinNotice(int senderFd, const std::string& nick)
{
	std::string msg = ":" + nick + " JOIN " + name + "\r\n";
	for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		if (*it != senderFd)
		{
			try
			{
				sendMsg(*it, msg);
			}
			catch (const std::runtime_error& e)
			{
				std::cerr << "[join notice send error] " << e.what() << " (fd: " << *it << ")\n";
			}
		}
	}
}

const std::set<int>& Channel::getMembers() const { return members; }

void Channel::lockTopic(bool mode) { topicLock = mode; }

bool Channel::isTopicLocked() const { return topicLock; }

void Channel::enablePassword(bool mode) { hasPassword = mode; }

bool Channel::isPasswordEnabled() const { return hasPassword; }

void Channel::enableLimit(bool mode) { limitEnabled = mode; }

bool Channel::isLimitEnabled() const { return limitEnabled; }

std::string Channel::getModeString() const
{
	std::string modes = "+";
	if (inviteOnly) modes += "i";
	if (topicLock) modes += "t";
	if (hasPassword) modes += "k";
	if (limitEnabled) modes += "l";
	return modes;
}
