# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/23 13:24:02 by amaucher          #+#    #+#              #
#    Updated: 2024/06/04 23:09:13 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SOURCES = ./src/main.c ./src/utils/list_operators.c ./src/utils/itoa.c \
		./src/lexer/initialization.c ./src/lexer/env_lib.c ./src/utils/free_memory.c \
		./src/lexer/lexer_cleanup.c ./src/lexer/lexer_env.c ./src/lexer/lexer_quotes.c \
		./src/lexer/lexer_tokens.c ./src/lexer/lexer_word_token.c ./src/lexer/lexer.c \
		./src/utils/str_utils_2.c ./src/utils/list_operators_2.c ./src/utils/testing.c \
		./src/utils/free_memory_2.c ./src/utils/ft_split.c ./src/utils/str_utils.c \
		./src/parser/parser.c ./src/parser/command_list.c ./src/parser/pipe_and_word.c \
		./src/parser/redirections.c ./src/parser/input_check.c ./src/executor/executor.c \
		./src/builtins/builtins.c ./src/builtins/echo.c ./src/builtins/pwd.c ./src/builtins/unset.c \
		./src/builtins/cd.c ./src/builtins/exit.c ./src/builtins/env.c ./src/builtins/export.c \
		./src/utils/signals.c ./src/lexer/lexer_env_path.c ./src/builtins/export_env.c \
		./src/builtins/cd_utils.c ./src/executor/error_handling.c \
		./src/executor/file_handling.c ./src/utils/free_memory_4.c ./src/utils/str_utils_3.c \
		./src/executor/in_redirections.c ./src/executor/out_redirections.c ./src/executor/heredocs.c \
		./src/executor/pipes.c ./src/executor/find_path.c ./src/executor/terminate_processes.c \
		./src/executor/misc.c ./src/utils/free_memory_3.c 
		

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
