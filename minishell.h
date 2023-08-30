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
#include <readline/readline.h>
#include <readline/history.h>

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
}	t_type;
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

typedef struct s_data
{
    int tokens;
    t_list *list;
    char *line;
    char *string_between_quotes;
	char 	*str_with_all_tokens;
    t_type token_type;
    t_token token;
}   t_data;

//parser_utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
int	ft_strcmp(const char *s1, const char *s2);

//list operators
void	printlist(t_list *head);

#endif
