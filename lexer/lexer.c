/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

// add multiple checks for all kind of delimiters e.g. parameter, quotes, whitespaces
t_list *split_line_into_tokens(t_minishell m, char **envp)
{
	int 	i;

	i = 0;
	m.tlist = NULL;
	while (m.line[i])
	{
		if (m.line[i] == '|')
			m.str_with_all_tokens = pipe_token(&i, &m.token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			m.str_with_all_tokens = redirection_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '$')
			m.str_with_all_tokens = env_token(m.line, &i, &m.token_type, m.env_lib, envp);
		else if (m.line[i] == ' ' || m.line[i] == '\t')
			m.str_with_all_tokens = whitespace_token(m.line, &i, &m.token_type);
		else if (m.line[i] == '\'' || m.line[i] == '\"')
			m.str_with_all_tokens = single_or_double_quotes_token(m.line, &i, &m.token_type, m);
		else
			m.str_with_all_tokens = check_for_word_token(m.line, &i, &m.token_type);
		m.tlist = add_token_to_list(&m.tlist, m.str_with_all_tokens, m.token_type);
	}
	cleanup_token_list(m.tlist);
	return(m.tlist);
}