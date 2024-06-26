/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
in case of whitespace before the first command it needs to be skipped
*/
int	skip_whitespace(t_minishell m)
{
	int	i;

	i = 0;
	while (m.line[i] == ' ' || m.line[i] == '\t')
	{
		i++;
	}
	return (i);
}

/*
in case the amount of double quotes is uneven an error message is
printed to the terminal
*/
char	*check_uneven_amount_of_quotes(char *line, t_minishell *m)
{
	int	counter;
	int	i;

	(void)m;
	i = 0;
	counter = 0;
	while (line[i])
	{
		if (line[i] == '"')
			counter++;
		i++;
	}
	if (counter % 2 != 0)
	{
		printf("uneven amount of quotes: dquotes not handled\n");
	}
	return (NULL);
}

void	check_for_single_dot(char *line, int *i)
{
	(void) i;
	if (line[0] == '.' && line[1] == '\0')
	{
		errno = 2;
		printf("Error code: %d, Error message: %s\n", 
			errno, strerror(errno));
	}
}

/*
iterates through line and splits it into tokens;
splitting occurs whenever a symbol specified in the if conditions appears;
the line is iterated (i) within the token functions;
*/
t_list	*split_line_into_tokens(t_minishell m)
{
	int	i;

	m.tlist = NULL;
	i = skip_whitespace(m);
	check_uneven_amount_of_quotes(m.line, &m);
	check_for_single_dot(m.line, &i);
	while (m.line[i])
	{
		if (m.line[i] == '|')
			m.token_str = pipe_token(&i, &m.token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			m.token_str = redirection_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '$')
			m.token_str = env_token(&i, &m.token_type, m.envp, &m);
		else if (m.line[i] == ' ' || m.line[i] == '\t')
			m.token_str = whitespace_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '\'' || m.line[i] == '\"')
			m.token_str = quotes(m.line, &i, &m.token_type, m);
		else
			m.token_str = check_for_word_token(m.line, &i, &m.token_type);
		m.tlist = add_token_to_list(&m.tlist, m.token_str, m.token_type);
	}
	cleanup_token_list(m.tlist);
	return (m.tlist);
}
