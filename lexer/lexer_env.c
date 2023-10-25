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

bool	check_if_part_of_library(t_list *envp, char *search_str)
{
	t_list	*tmp;
	t_dict	*dict;
	int		i;

	i = 0;
	tmp = envp;
	dict = (t_dict *)tmp->value;
	while (tmp != NULL)
	{
		dict = tmp->value;
		if (ft_strcmp(dict->key, search_str) == 0)
			return (true);
		i++;
		tmp = tmp->next;
	}
	return (false);
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
extract the env_str from the input;
if theres a single dollar sign it's supposed to be printed
*/
char	*env_token(char *line, int *i, t_type *token_type,
			t_list *env_list)
{
	char	*search_str;
	char	*env_final;

	(*i)++;
	*token_type = WORD;//ENV;
	env_final = NULL;
	search_str = extract_env_name(line, i);
	printf("SEARCH STR %s\n", search_str);
	if (check_if_part_of_library(env_list, search_str) == false)
	{
		if (ft_strcmp(search_str, "$") == 0)
		{
			*token_type = WORD;
			free(search_str);
			return(ft_substr("$", 0, 1));
		}
		else
		{
			*token_type = ENV_FAIL;
			free(search_str);
			return (ft_substr("", 0, 1)); //! does this make sense? how should I process it?
			//! if no matching ENV nothing happens, meaning I could do if type ENV_FAIL = skip that command
		}
	}
	else 
	{
		env_final = *find_path_after_key(env_list, search_str);
		free(search_str);
	}
	return (env_final);
}