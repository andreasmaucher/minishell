/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:53:44 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 18:11:02 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**find_path_executor(t_list *envp)
{
	char	*path;
	char	**path_buf;
	t_list	*tmp;
	t_dict	*dict;

	tmp = envp;
	dict = (t_dict *)tmp->value;
	while (tmp != NULL 
		&& ft_strnstr(dict->value, "PATH", ft_strlen("PATH")) == NULL)
	{
		dict = tmp->value;
		tmp = tmp->next;
	}
	path = ft_strstr(dict->value, "=");
	if (path == NULL)
		return (NULL);
	path_buf = ft_split(++path, ':');
	return (path_buf);
}

char	*valid_path(char **path, char *argv)
{
	int		i;
	char	*correct_path;

	i = 0;
	correct_path = NULL;
	if (argv != NULL && (argv[0] == '.' || argv[0] == '/'))
	{
		if (check_if_file_can_be_opened(argv) == 0)
			return (argv);
		else
			return (NULL);
	}
	while (path[i])
	{
		correct_path = join_strings(path[i], "/", argv);
		if (check_if_file_can_be_opened(correct_path) == 0)
			return (correct_path);
		else
			free (correct_path);
		i++;
	}
	return (NULL);
}
