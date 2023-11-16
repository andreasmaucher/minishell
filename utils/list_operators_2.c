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

t_token	*add_token_type_and_str(char *token_str, t_type token_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = token_str;
	token->type = token_type;
	return (token);
}

t_list	*find_previous_node(t_list *head, t_list *target_node)
{
	t_list	*previous_node;
	t_list	*current_node;

	current_node = head;
	previous_node = NULL;
	while (current_node != NULL && current_node != target_node)
	{
		previous_node = current_node;
		current_node = current_node->next;
	}
	return (previous_node);
}

void	ft_lstremove(t_list **lst, t_list *node, void (*del)(void *))
{
	t_list	*prev;

	if (!lst || !(*lst) || !node)
		return ;
	prev = find_previous_node(*lst, node);
	if (!prev)
		*lst = node->next;
	else
		prev->next = node->next;
	ft_lstdelone(node, del);
}

/*
creates a new node for in_filenames(that is to be added to a list 
in subsequent function)
*/
t_list	*create_new_filename_node(void *value, char *eof)
{
	t_list	*newnode;
	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->eof = eof;
	newnode->is_heredoc = 0;
	newnode->next = NULL;
	return (newnode);
}

/*
creates a new node for in_filenames(that is to be added to a
list in subsequent function)
*/
t_list	*create_new_append_node(void *value)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->is_append = 0;
	newnode->next = NULL;
	return (newnode);
}
