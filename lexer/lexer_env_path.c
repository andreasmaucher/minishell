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
searches for the corresponding env to search_str within envp;
when assigning to the buffer path is iterated by one to skip the equal
sign
*/

char	**find_path(char **envp, char *search_str)
{
	int		i;
	char	*path;
	char	**path_buf;

	i = 0;
	while (ft_strnstr(envp[i], search_str, ft_strlen(search_str)) == NULL)
		i++;
	path = ft_strstr(envp[i], "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, '\0');
	return (path_buf);
}

char	**find_path_after_key(t_list *envp, char *search_str)
{
	char	*path;
	char	**path_buf;
	t_list	*tmp;
	t_dict	*dict;

	tmp = envp;
	dict = (t_dict *)tmp->value;
	while (ft_strnstr(dict->value, search_str, ft_strlen(search_str)) == NULL)
	{
		tmp = tmp->next;
		dict = tmp->value;
	}
	path = ft_strstr(dict->value, "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, '\0');
	return (path_buf);
}