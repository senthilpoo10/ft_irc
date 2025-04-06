# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pchennia <pchennia@student.42.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 13:07:43 by pchennia          #+#    #+#              #
#    Updated: 2025/04/06 10:30:08 by pchennia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := irc
CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++11 -g

SRCS = src/main.cpp src/Client.cpp src/Server.cpp 
OBJS := $(SRCS:.cpp=.o)

GREEN = \033[0;32m
YELLOW = \033[0;33m
CYAN_B = \033[1;36m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)Created $(NAME)$(RESET)"

%.o: %.cpp
	@clear
	@echo "$(CYAN_B)Compiling... $(NAME)$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	@echo "$(YELLOW)Removed Object files$(RESET)"

fclean: clean
	rm -rf $(NAME)
	@echo "$(YELLOW)Removed $(NAME)$(RESET)"

re: fclean all

.PHONY: all clean fclean re
