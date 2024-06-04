/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
returns the path after the key, meaning everything after '='
*/
void	find_path_after_key_tilde(t_list *envp, char *search_str)
{
	char	*path;
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
		return ;
	path++;
	printf("%s", path);
}

void	echo_print(t_command *cmd, int *i, t_minishell m)
{
	while (cmd->args[(*i)] != NULL)
	{
		if (ft_strcmp(cmd->args[(*i)], "~") == 0)
			find_path_after_key_tilde(m.envp, "HOME");
		printf("%s", cmd->args[(*i)]);
		if (cmd->args[(*i) + 1] != NULL)
			printf(" ");
		(*i)++;
	}
}

/*
Expected behavior:
- 'echo': prints the provided text to the terminal followed by a newline
- 'echo -n': prints the provided text to the terminal but suppresses newline, -n
needs to be the first word after echo, if not it's simply 'echoed';
- i starts at 1 to skip the actual command name and 2 in case -n flag is set;
- '~' can occur at any point after echo and prints the home directory path;
*/

int	echo(t_minishell m, t_command *cmd)
{
	bool	n_flag;
	int		i;
	int		j;

	j = 0;
	i = 1;
	n_flag = false;
	if (cmd->args[i] != NULL)
	{
		if (cmd->args[1][j] == '-' && cmd->args[1][++j] == 'n')
		{
			while (cmd->args[1][j] == 'n' && cmd->args[1][j])
				j++;
			if (cmd->args[1][j] == '\0')
			{
				n_flag = true;
				i = 2;
			}
		}
		echo_print(cmd, &i, m);
	}
	if (n_flag == false || cmd->args[1] == NULL)
		printf("\n");
	return (0);
}
