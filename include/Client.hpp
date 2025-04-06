/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchennia <pchennia@student.42.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:44:27 by pchennia          #+#    #+#             */
/*   Updated: 2025/04/06 22:39:20 by pchennia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Headers.hpp"

class Client
{
    private:
        int fdClient;  
        std::string NickName;
        std::string UserName;
        std::string FullName;
        std::string prefix;
        std::string registredTime;
        std::string ip; // IP address of the client
        int limitChannel;
        bool Authenticate;  // After successful PASS command
        bool registred; // After successful NICK and USER commands
        bool isOper;    // If the user has logged in as an operator

    public:
        Client();
        Client(int fd);
        ~Client();

        int getFdClient();
        
        void    setAuth(bool auth);
        bool    getAuth();

        void setRegistred(bool reg);
        bool getRegistred();

        void    setPrefix(std::string pre);
        std::string getPrefix();

        void    setNickName(std::string nick);
        std::string getNickName();

        void    setUserName(std::string user);
        std::string getUserName();

        void    setFullName(std::string full);
        std::string getFullName();

        void    setOper(bool oper);
        bool    getOper();

        void    setTime(std::string time);
        std::string getTime();

        void    setIp(std::string ip);
        std::string    getIp();
};

#endif