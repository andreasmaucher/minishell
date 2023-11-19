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

int	count_cd_args(t_minishell *m, t_command *cmd)
{
	if ((arg_count(cmd->args) > 2))
	{
		printf("Too many arguments\n");
		m->status_code2 = 1;
		return (m->status_code);
	}
	return (0);
}

char	*go_back_to_home(t_minishell *m, char *path)
{
	path = get_path(m, "HOME");
	if (!path)
	{
		printf(("Error HOME not set!\n"));
		return (NULL);
	}
	return (path);
}

char	*go_back_to_last_directory(t_minishell *m, char *path)
{
	path = get_path(m, "OLDPWD");
	if (!path)
	{
		printf(("Error OLDPWD not set!\n"));
		return (NULL);
	}
	return (path);
}

/*
deletes a node with the specified search_key in the env list
*/
void	delete_node(t_minishell *m, char *search_key)
{
	t_list	*current;
	t_list	*prev;
	t_dict	*dict;

	current = m->envp;
	prev = NULL;
	while (current != NULL)
	{
		dict = (t_dict *)(current->value);
		if (dict != NULL && ft_strcmp(dict->key, search_key) == 0) 
		{
			if (prev != NULL) 
				prev->next = current->next;
			else 
				m->envp = current->next;
			free(dict->key);
			free(dict->value);
			free(dict);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
adds a new node to the env list
*/
void	add_specific_envs(t_minishell *m, char *path, char *key)
{
	t_list	*new;
	t_dict	*dict;
	char	*key_and_path;
	char	*key_and_equal;

	new = NULL;
	dict = NULL;
	new = create_new_node(NULL);
	dict = malloc(sizeof(t_dict));
	key_and_equal = ft_strjoin(key, "=");
	key_and_path = ft_strjoin(key_and_equal, path);
	dict->value = key_and_path;
	dict->key = ft_strdup(key);
	new->value = dict;
	free(key_and_equal);
	insert_at_tail(m->envp, new);
}
