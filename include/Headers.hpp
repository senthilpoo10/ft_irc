/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchennia <pchennia@student.42.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:44:59 by pchennia          #+#    #+#             */
/*   Updated: 2025/04/06 23:10:11 by pchennia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
# define HEADERS_HPP

#include <cstdlib>	// for exit()
#include <cstring>   // for memset
#include <csignal>   // for signal, SIGINT, SIGTSTP, SIGPIPE, SIG_IGN
#include <sys/types.h>	// Basic system types
#include <sys/socket.h>	// socket(), bind(), AF_INET, SOCK_STREAM, etc.
#include <netdb.h>	// getaddrinfo(), struct addrinfo, AI_PASSIVE
#include <iostream>
#include <unistd.h>	// for close()
#include <poll.h>	// for struct pollfd, POLLIN
#include <vector>
#include <fcntl.h>	// for fcntl, F_SETFL, O_NONBLOCK
#include <arpa/inet.h>	// inet_ntop(), htons()
#include <map> // for map

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#endif