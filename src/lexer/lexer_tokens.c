/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*char_to_str(char c)
{
	char	*str;
	int		i;

	str = malloc(sizeof(char) * 2);
	if (str == NULL)
		return (NULL);
	i = 0;
	str[i] = c;
	str[++i] = '\0';
	return (str);
}

/*
function that gets called in case a redirection token was encountered in 
the input string;determines the redirection type depending on the input and
returns a duplicate 
*/
char	*redirection_token(char *line, int *i, t_type *token_type)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		*token_type = REDIRECT_HEREDOC;
		(*i) += 2;
		return (ft_strdup("<<"));
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		*token_type = REDIRECT_APPEND;
		(*i) += 2;
		return (ft_strdup(">>"));
	}
	else if (line[*i] == '>')
	{
		*token_type = REDIRECT_OUT;
		(*i) += 1;
		return (ft_strdup(">"));
	}
	else
	{
		*token_type = REDIRECT_IN;
		(*i) += 1;
		return (ft_strdup("<"));
	}
}

/*
function that gets called when a pipe token was detected in the
input string; it sets the token type and returns a duplicate
*/
char	*pipe_token(int *i, t_type *token_type)
{
	*token_type = PIPE;
	(*i)++;
	return (ft_strdup("|"));
}

/*
function iterates i as long as whitespace is encountered;
*/
char	*whitespace_token(char *line, int *i, t_type *token_type)
{
	int	start_index;
	int	end_index;

	*token_type = WHITESPACE;
	start_index = *i;
	while ((line[*i] == ' ' || line[*i] == '\t') && line[*i])
		(*i)++;
	end_index = *i - start_index;
	return (ft_substr(line, start_index, end_index));
}
