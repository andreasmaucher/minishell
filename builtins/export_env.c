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
checks if the current key matches with the key in the user input;
if true the corresponding node is deleted and free 
*/
bool	check_for_key_doubles(t_minishell *m, char *search_str, t_list *tmp)
{
	t_dict	*dict;

	dict = (t_dict *)tmp->value;
	if (ft_strcmp(dict->key, search_str) == 0)
	{
		printf("DELETE DOUBLE KEY\n");
		ft_lstremove(&m->envp, tmp, delete_envp);
		return (true);
	}
	return (false);
}

t_list	*delete_double_envs(t_minishell *m, t_command *cmd)
{
	int		i;
	t_list	*tmp;
	char	*search_str;

	i = 0;
	tmp = m->envp;
	search_str = NULL;
	while (cmd->args[i] != NULL)
	{
		tmp = m->envp;
		search_str = extract_key_from_envp(cmd->args[i]);
		if (check_if_part_of_library(m->envp, search_str) == true)
		{
			while (tmp != NULL)
			{
				if (check_for_key_doubles(m, search_str, tmp) == true)
					break ;
				tmp = tmp->next;
			}
		}
		free(search_str);
		i++;
	}
	return (tmp);
}

void	add_new_envs(t_minishell *m, t_command *cmd)
{
	int		i;
	t_list	*new;
	t_dict	*dict;

	i = 0;
	new = NULL;
	dict = NULL;
	while (cmd->args[i])
	{
		if (check_equal_sign(cmd->args[i]) == true)
		{
			new = create_new_node(NULL);
			dict = malloc(sizeof(t_dict));
			dict->value = ft_strdup(cmd->args[i]);
			dict->key = extract_key_from_envp(cmd->args[i]);
			new->value = dict;
			insert_at_tail(m->envp, new);
		}
		i++;
	}
}
