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

/*
delete all ENV_FAIL type tokens at the end of the lexer
*/
t_list	*delete_env_fail(t_list **tlist, t_list *current_node)
{
	t_token	*tmp_token;

	tmp_token = current_node->value;
	if (tmp_token->type == ENV_FAIL)
		ft_lstremove(tlist, current_node, delete_token);
	return (*tlist);
}

/*
checks if the env is part of the library 
*/
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

/*
returns the path after the key, meaning everything after '='
*/
char	*find_path_after_key(t_list *envp, char *search_str)
{
	char	*path;
	char	*path_buf;
	t_list	*tmp;
	t_dict	*dict;

	tmp = envp;
	dict = (t_dict *)tmp->value;
	while (ft_strnstr(dict->value, search_str, ft_strlen(search_str)) == NULL)
	{
		tmp = tmp->next;
		dict = tmp->value;// maybe change the order?
	}
	path = ft_strstr(dict->value, "=");
	if (path == NULL)
		return (NULL);
	path++;
	path_buf = ft_strdup(path);
	return (path_buf);
}
