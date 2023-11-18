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

#include "../minishell.h"

char	*env_within_double_quotes(char *line, int *i)
{
	int		start;
	int		length;
	char	*env_string;

	start = ++(*i);
	if (line[*i] == '\0' || line[*i] == '"')
		return (ft_strdup("$"));
	while (line[*i] && check_for_metacharacter(line[*i]) == false
		&& line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
		(*i)++;
	length = *i - start;
	env_string = malloc(sizeof(char) * (length + 1));
	if (!env_string)
		return (NULL);
	strncpy(env_string, &line[start], length);
	env_string[length] = '\0';
	return (env_string);
}

/* search_str is the string that needs to be found */
char	*extract_env_name(char *line, int *i)
{
	int		start;
	int		length;
	char	*search_str;

	start = *i;
	if (!line[*i] || line[*i] == '"')
		return (ft_strdup("$"));
	while (line[*i] != '\0' && check_for_metacharacter(line[*i]) == false
		&& check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	length = *i - start;
	search_str = ft_substr(line, start, length);
	return (search_str);
}

/*
for '$' variables a dollar sign is printed and the type is set to word
*/
char	*handle_dollar_signs(t_type *token_type, char *search_str)
{
	*token_type = WORD;
	free(search_str);
	return (ft_substr("$", 0, 1));
}

/*
in case the env is not part of the library its type is set to ENV_FAIL
and the node is later on deleted;
if it is part of the library the corresponding path for the key is 
searched for and returned;
*/
char	*env_token_expansion(char *line, int *i, t_type *token_type,
			t_list *env_list)
{
	char	*search_str;
	char	*env_final;

	search_str = extract_env_name(line, i);
	env_final = NULL;
	if (check_if_part_of_library(env_list, search_str) == false)
	{
		if (ft_strcmp(search_str, "$") == 0)
			return (handle_dollar_signs(token_type, search_str));
		else
		{
			*token_type = ENV_FAIL;
			free(search_str);
			return (ft_substr("\0", 0, 0));
		}
	}
	else 
	{
		env_final = find_path_after_key(env_list, search_str);
		free(search_str);
	}
	return (env_final);
}

/*
extract the env_str from the input;
if theres a single dollar sign it's supposed to be printed;
'$?' -> returns the last exit code;
*/
char	*env_token(char *line, int *i, t_type *token_type,
			t_list *env_list, t_minishell *m)
{
	char	*env_final;
	int		j;

	(*i)++;
	*token_type = WORD;
	env_final = NULL;
	j = (*i);
	if (line[*i] == '?' && (line[j + 1] == 32 || line[j + 1] == '\0'))
	{
		(*i)++;
		env_final = ft_itoa(m->status_code);
		return (env_final);
	}
	else
		env_final = env_token_expansion(line, i, token_type, env_list);
	return (env_final);
}
