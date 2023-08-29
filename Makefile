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
SOURCES = main.c parser_utils.c

OBJS = $(SOURCES:.c=.o)

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -lreadline

all: $(NAME)	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLXFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
