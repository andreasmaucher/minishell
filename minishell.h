/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:16:54 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:16:56 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pipex/pipex.h"

//token type (index starts at 0 e.g. word = 0, pipe = 2)
typedef enum
{
	WORD,
	WHITESPACE,
    PIPE,
    REDIRECT_HEREDOC,
    REDIRECT_APPEND,
    REDIRECT_IN,
    REDIRECT_OUT,
    DOUBLE_QUOTES,
    SINGLE_QUOTES,
	ENV,
}	t_type;

typedef struct s_dict
{
	char	*key;
	void	*value;
}	t_dict;
typedef struct s_list
{
	void *value; //value needs to be void since we want to be able to store multiple different types
	struct s_list	*next;
}	t_list;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;

//parser
typedef enum cmd_type
{
	BUILTIN,
	PATH,
}	t_cmd_type; //!

typedef struct s_file
{
	int		fd;
	char	*text_to_file;
	char	*stop_heredoc;
	t_type	redirection_type;
	char	*new_heredoc_file;
}	t_file;
typedef struct s_command //!
{
	t_list			*arguments;
	t_cmd_type		type;
	bool			before_pipe;
	bool			after_pipe;
	t_type	        input_redir_or_heredoc;
	t_type	        out_redir_type;
	t_list			*inred_file;
	t_list			*outred_file;
}					t_command;

typedef struct s_minishell
{
    int tokens;
    t_list *tlist;
    t_list *clist;
	t_list	*env;
    char *line;
    char *string_between_quotes;
	char 	*str_with_all_tokens;
    t_type token_type;
    t_token token;
}   t_minishell;

//utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
int	    ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	**ft_split(char const *s, char c);

//list operators
void	printlist(t_list *head);
t_list	*create_new_node(void *value);
void	insert_at_tail(t_list *head, t_list *new_value);

//parser
t_list *parser(t_minishell m);

//itoa
char	*ft_itoa(int n);

#endif
