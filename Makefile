# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/23 13:24:02 by amaucher          #+#    #+#              #
#    Updated: 2023/08/23 13:24:07 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SOURCES = main.c utils.c list_operators.c parser.c itoa.c

PIPEX = pipex/pipex.o
OBJS = $(SOURCES:.c=.o)
INCLUDES = -I pipex/includes

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -lreadline

all: $(NAME)	

$(NAME): $(PIPEX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLXFLAGS)

$(PIPEX):
	make -C pipex

clean:
	make -C pipex clean
	$(RM) $(OBJS)

fclean: clean
	make -C pipex fclean
	$(RM) $(NAME)

re: fclean $(NAME)
