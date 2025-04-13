/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_poonkodi.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:41:36 by pchennia          #+#    #+#             */
/*   Updated: 2025/04/13 21:40:31 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
    this->Authenticate = false;
}
Client::Client(int fd)
{
    this->Authenticate = false;
    this->registred = false;
    this->isOper = false;
    this->fdClient = fd;
    this->limitChannel = 3;
}

int Client::getFdClient() {return this->fdClient;};

void Client::setAuth(bool auth) {this->Authenticate = auth;}
bool Client::getAuth() {return this->Authenticate;}

void Client::setRegistred(bool reg) {this->registred= reg;}
bool Client::getRegistred() {return this->registred;}

// void    Client::setNickName(std::string nick) {this->NickName = nick;}
// std::string Client::getNickName() {return this->NickName;}

// void    Client::setUserName(std::string user) {this->UserName = user;}
// std::string Client::getUserName() {return this->UserName;}

void    Client::setFullName(std::string full) {this->FullName = full;}
std::string Client::getFullName() {return this->FullName;}

void    Client::setOper(bool oper) {this->isOper = oper;}
bool    Client::getOper() {return this->isOper;}

void    Client::setPrefix(std::string pre) {this->prefix = pre;}
std::string Client::getPrefix() {return this->prefix;}

void    Client::setTime(std::string time) {this->registredTime = time;}
std::string Client::getTime() {return this->registredTime;}

void    Client::setIp(std::string ip) {this->ip = ip;};
std::string Client::getIp() {return this->ip;};


Client::~Client()
{
    
}







//Added for channel
void Client::setNickname(const std::string& nick) { _nickname = nick; }
std::string Client::getNickname() const { return _nickname; }

void Client::setUsername(const std::string& name) { _username = name; }
std::string Client::getUsername() const { return _username; }