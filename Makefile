# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/23 13:24:02 by amaucher          #+#    #+#              #
#    Updated: 2023/11/22 00:22:04 by mrizakov         ###   ########.fr        #
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
		builtins/builtins.c builtins/echo.c builtins/pwd.c builtins/unset.c \
		builtins/cd.c builtins/exit.c builtins/env.c builtins/export.c \
		utils/signals.c lexer/lexer_env_path.c builtins/export_env.c \
		builtins/cd_utils.c executor/error_handling.c \
		executor/file_handling.c utils/free_memory_4.c utils/str_utils_3.c \
		executor/in_redirections.c executor/out_redirections.c executor/heredocs.c \
		executor/pipes.c executor/find_path.c executor/terminate_processes.c \
		executor/misc.c
		

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

.PHONY: all clean fclean re
