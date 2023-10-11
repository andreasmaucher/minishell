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

# include "../minishell.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == 0)
		return (0);
	while (lst->next != 0)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*prev;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		prev = ft_lstlast(*lst);
		prev->next = new;
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == 0)
		return (0);
	node->value = content;
	node->next = 0;
	return (node);
}

/* adds a new node to a list; in case the list is empty, the new node becomes the head, else 
it is added at the end of the list */
t_list *create_command_list(t_list **clist, t_command *tmp_cmd)
{
	t_list *new_node;
	
	new_node = create_new_node(tmp_cmd);
	if (!*clist)
		*clist = new_node;
	else
		insert_at_tail(*clist, new_node);
	return (*clist);
}

/* initialization of clist to NULL, to ensure that it starts as an empty list */
t_list *setup_command_list(t_list **clist, t_list *tlist)
{
    int len;
    t_command   *tmp_cmd;

    *clist = NULL;
    len = command_count(tlist);
    while (len--)
    {
        tmp_cmd = ft_create_cmd();
        *clist = create_command_list(clist, tmp_cmd);
    }
    return(*clist);
}