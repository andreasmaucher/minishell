NAME = minishell
SOURCES = lexer.c

OBJS = $(SOURCES:.c=.o)

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -lreadline -lhistory

all: $(NAME)	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLXFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)