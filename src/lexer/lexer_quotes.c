/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_str(char *str, char *appendix)
{
	char	*appended_str;

	if (!str)
		return (appendix);
	appended_str = ft_strjoin(str, appendix);
	free(appendix);
	free(str);
	return (appended_str);
}

char	*handle_dollar_quotes(char *line, int *i, t_minishell m, 
			char *str_between_quotes)
{
	char	*search_str;
	char	*env_expanded;

	env_expanded = NULL;
	if (line[*i] == '$')
	{
		(*i)++;
		search_str = extract_env_name(line, i);
		if (check_if_part_of_library(m.envp, search_str) == true)
			env_expanded = find_path_after_key(m.envp, search_str);
		else if (check_if_part_of_library(m.envp, search_str) == false)
		{
			if (ft_strcmp(search_str, "$") == 0)
				env_expanded = ft_strdup("$");
			else
				env_expanded = ft_strdup("");
		}
		str_between_quotes = append_str(str_between_quotes, env_expanded);
		free(search_str);
	}
	else
		str_between_quotes = append_str(str_between_quotes,
				char_to_str(line[(*i)++]));
	return (str_between_quotes);
}

char	*double_quote_to_string(char *line, int *i, t_minishell m)
{
	char	*str_between_quotes;

	str_between_quotes = NULL;
	while (line[*i] != '"' && line[*i] != '\0')
		str_between_quotes = handle_dollar_quotes(line, i, m,
				str_between_quotes);
	if (line[*i])
		(*i)++;
	return (str_between_quotes);
}

char	*single_quote_to_string(char *line, int *i)
{
	char	*str_between_quotes;
	int		start;
	int		length;

	start = *i;
	length = 0;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	length = *i - start;
	str_between_quotes = malloc(sizeof(char) * (length + 1));
	if (!str_between_quotes)
		return (NULL);
	ft_strncpy(str_between_quotes, &line[start], length);
	str_between_quotes[length] = '\0';
	if (line[*i])
		(*i)++;
	return (str_between_quotes);
}

/*
depending if the symbol is double or single quotes the respective
function is called; before entering the function i is increased by one;
*/
char	*quotes(char *line, int *i,
				t_type *token_type, t_minishell m)
{
	if (line[*i] == '"')
	{
		*token_type = WORD;
		if (line[*i])
			(*i)++;
		return (double_quote_to_string(line, i, m));
	}
	else
	{
		*token_type = WORD;
		if (line[*i])
			(*i)++;
		return (single_quote_to_string(line, i));
	}
}
