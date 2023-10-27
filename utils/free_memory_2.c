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

/* delete a node in the env list */
void	delete_envp(void *arg)
{
	t_dict	*dict;

	if (!arg)
		return ;
	dict = (t_dict *)arg;
	if (!dict)
		return ;
	dict->value = set_pt_to_null(dict->value);
	dict->key = set_pt_to_null(dict->key);
	dict = set_pt_to_null(dict);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

/*
deletes a single node within a linked list and frees the associated memory
*/
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!del)
		return ;
	del(lst->value);
	free(lst);
}

/*
deletes an entire linked list
*/
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!del)
		return ;
	while (lst && *lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
}

void	free_all(t_minishell m)
{
	if (m.line)
		m.line = set_pt_to_null(m.line);
	if (m.tlist)
		ft_lstclear(&m.tlist, delete_token);
	if (m.clist)
		ft_lstclear(&m.clist, delete_cmd);
	if (m.envp)
		ft_lstclear(&m.envp, delete_envp);
}
