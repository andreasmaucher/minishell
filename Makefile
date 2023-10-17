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
SOURCES = main.c utils/list_operators.c utils/itoa.c \
		lexer/initialization.c lexer/env_lib.c utils/free_memory.c \
		lexer/lexer_cleanup.c lexer/lexer_env.c lexer/lexer_quotes.c \
		lexer/lexer_tokens.c lexer/lexer_word_token.c lexer/lexer.c \
		utils/str_utils_2.c utils/list_operators_2.c utils/testing.c \
		utils/free_memory_2.c utils/ft_split.c utils/str_utils.c \
		parser/parser.c parser/command_list.c parser/pipe_and_word.c \
		parser/redirections.c parser/input_check.c executor/executor.c \

OBJS = $(SOURCES:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3
MLXFLAGS = -lreadline

all: $(NAME)	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLXFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
