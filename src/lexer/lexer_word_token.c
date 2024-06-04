/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_for_metacharacter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}

/* checks if c is a double or single quote */
bool	check_for_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (true);
	else
		return (false);
}

/* each substring is malloced! */
char	*check_for_word_token(char *line, int *i, t_type *token_type)
{
	int	start_index;
	int	end_index;

	*token_type = WORD;
	start_index = *i;
	while (check_for_metacharacter(line[*i]) == false
		&& check_for_quotes(line[*i]) == false && line[*i] != '$' && line[*i])
		(*i)++;
	end_index = *i - start_index;
	return (ft_substr(line, start_index, end_index));
}
