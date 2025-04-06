/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchennia <pchennia@student.42.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:12:27 by pchennia          #+#    #+#             */
/*   Updated: 2025/03/13 13:15:59 by pchennia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int isValidArgs(char **av) {
    std::string port = av[0];
    std::string password = av[1];

    if (port.empty() || password.empty())
        return 1;
    for (size_t i = 0; i < port.length(); i++) {
        if (!std::isdigit(port[i]))
            return 1;
    }
    for (size_t i = 0; i < password.length(); i++) {
        if (std::isspace(password[i]))
            return 1;
    }
    int portNum = std::atoi(av[0]);
    if (portNum == 0 || portNum <= 1023)
        return 1;
    else if (portNum >= 65535)
        return 1;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3 || isValidArgs(&argv[1])){
        std::cout << "USAGE : " << RED << "./irc_serv [port] [password]" << RESET << std::endl;
        return 1;
    }
    Server server(argv[1], argv[2]);
    server.startServer();
}
