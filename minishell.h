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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_data
{
    int tokens;
}   t_data;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;
typedef struct s_list
{
	void *value; //value needs to be void since we want to be able to store multiple different types
	struct s_list	*next;
}	t_list;

//token type
typedef enum type
{
	WORD,
	WHITESPACE,
    PIPE,
    REDIRECT_HEREDOC,
    REDIRECT_APPEND,
    REDIRECT_IN,
    REDIRECT_OUT,
}	t_type;

//parser_utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);

//list operators
void	printlist(t_list *head);


#endif
